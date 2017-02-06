#ifndef CALL_EXECUTOR_H
#define CALL_EXECUTOR_H
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <queue>
#include <deque>
//#include <set>

#include <assert.h>
#include <mutex>
#include <iostream>
#include "common.h"


typedef  std::lock_guard<std::mutex> default_mlock;
typedef std::deque<CServerStatus> server_status_queue;

//----------------------------------------------------------------------

//----------------------------------------------------------------------
using namespace std;

struct CompaniesSorter
{
    bool operator () (CCompanyTask const & t1, CCompanyTask const & t2) const
    {
        if (t1.m_priority > t2.m_priority)
            return true;
        else if(t1.m_priority != t2.m_priority)
            return false;
        else
        {
            bool result = (t1.m_abonents.size() < t2.m_abonents.size());
            return result;
        }
    }
};

struct CHandleTaskSorter
{
    bool operator () (CTask_to_handle const & t1, CTask_to_handle const & t2) const
    {
        return m_cs(*t1.m_task, *t2.m_task);
    }
private:
    CompaniesSorter m_cs;
};

class caller_executor_pool;

class  executor_pool_base : public boost::noncopyable
{
public:
    virtual  void deliver_to_client(CInteractor_ptr client, CServerStatus const& msg) = 0;
    virtual ~executor_pool_base()
    {}
    virtual boost::asio::io_service& get_io_serv() = 0;
    virtual void CheckIfLeft(CInteractor_ptr client)  = 0;
};
class call_executor
    : //public CInteractor,
      public boost::enable_shared_from_this<call_executor>
{
public:
    call_executor(executor_pool_base & pool);
    virtual ~call_executor();



    void Proc(CTask_to_handle   task)
    {
        //m_pool.get_io_serv()
        m_io_service.post(boost::bind(&call_executor::CallCompanyTask, this, task));
        std::unique_ptr<boost::thread> t2(t.release());
        if (m_io_service.stopped() || !t2.get())// || !t2->joinable() )
        {
            if (m_io_service.stopped())
                m_io_service.reset();
            if (!t2->try_join_for(boost::chrono::milliseconds(0)))
                t2.reset(new boost::thread(boost::bind(&call_executor::Run, this)));
            else
                t.reset(t2.release());
        }

    }



private:
    void Run()
    {
        m_io_service.run();
        if (m_io_service.stopped())
        {
            m_io_service.reset();
        }
    }
    boost::asio::io_service   m_io_service;

    executor_pool_base & m_pool;
    std::unique_ptr<boost::thread> t;

    void CallCompanyTask(CTask_to_handle const& th);

};


class caller_executor_pool: public executor_pool_base
{

    caller_executor_pool( boost::asio::io_service & io_service_):
        m_io_service(io_service_), m_call_executor(*this)
    {

    }
    friend class call_server;
public:

    static char  const * endMessage ;//= "\n All tasks end";
    void join(CInteractor_ptr participant)
    {
        using namespace std;
        cout << __FUNCTION__ << " join" << endl;

    }

    void leave(CInteractor_ptr /*participant*/)
    {
        using namespace std;
        cout << __FUNCTION__ << " ." << endl;
    }

    void deliver_to_client(CInteractor_ptr client, CServerStatus const& msg) override
    {
        client->deliver(msg);
    }
    boost::asio::io_service& get_io_serv() override
    {
        return m_io_service;
    }

    void deliver(TInitiaWriteData & msg, CInteractor_ptr client)
    {

        CTask_to_handle task1;
        {
            std::lock_guard<std::mutex> lock(m_queue_m);
            // read_queue.emplace(*current_companyTask.release());

            //CallCompanyTask(msg);
            // std::move(msg.begin(), msg.end(), read_queue.end());

            while (!msg.empty())
            {
                //CCompanyTask & comp :
                read_queue.emplace(CTask_to_handle
                {
                    CCompanyTask_ptr(new CCompanyTask(std::move(msg.front()))), client});
                msg.pop_front();
            }
            // emplace(CTask_to_handle{ msg, client});


            task1 = read_queue.top();
            read_queue.pop();
        }
        m_call_executor.Proc(task1);

    }

    void CheckIfLeft(CInteractor_ptr client) override
    {

        bool tasks_left = false;
        {
            std::lock_guard<std::mutex> lock(m_queue_m);
            for (CTask_to_handle const& thi : read_queue.GetCont())
            {
                if (thi.m_client == client)//.get()
                {
                    tasks_left= true;
                    break;
                }

            }

        }
        if (!tasks_left)
        {
            cout << __FUNCTION__ << " no tasks_left ." << endl;

            client->deliver(CServerStatus(caller_executor_pool::endMessage)); // "All tasks end"

            CServerStatus endStatus (CServerStatus::getLastMark());
            client->deliver(endStatus);
        }
        else
        {
            client->deliver(CServerStatus("Have some your tasks"));


        }

        //load next
        {

            std::lock_guard<std::mutex> lock(m_queue_m);

            if (!read_queue.empty())
            {


                CTask_to_handle task1 =  read_queue.top();
                read_queue.pop();

                m_call_executor.Proc(task1);
            }
            // else
            // {   assert(task

        }


//        recent_msgs_.push_back(msg);
//        while (recent_msgs_.size() > max_recent_msgs)
//            recent_msgs_.pop_front();
//        m_client_app->deliver(msg);
        //  m_call_executor.
//        std::for_each(m_client_app.begin(), m_client_app.end(),
//                      boost::bind(&client_app::deliver, _1, boost::ref(msg)));
    }

private:
//client_app_ptr  m_client_app;
//holds pointers.
    //  std::set<CInteractor_ptr> m_client_sessions;
// std::set<CInteractor_weak_ptr> m_client_sessions;
// enum { max_recent_msgs = 100 };
    std::mutex m_queue_m;
    typedef    std::priority_queue<CTask_to_handle, std::deque<CTask_to_handle>, CHandleTaskSorter>
    TCont;

    struct UnProtectCont : public TCont
    {

        TCont::container_type const & GetCont() const
        {
            return c;
        }
    };
    UnProtectCont read_queue;



// std::set<CCompanyTask, CompaniesSorter>
    // std::set<CTask_to_handle,  CHandleTaskSorter>    read_queue;
// server_status_queue recent_msgs_;
    boost::asio::io_service & m_io_service;
    call_executor m_call_executor;
};



#endif // CALL_EXECUTOR_H
