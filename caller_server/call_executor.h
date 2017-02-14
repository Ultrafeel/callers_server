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
int procTC();
//----------------------------------------------------------------------
using namespace std;

struct CompaniesSorter
{
    bool operator () (CCompanyTask const & t1, CCompanyTask const & t2) const
    {
        return (compar(t1, t2) > 0);
    }
//protected:
    int compar (CCompanyTask const & t1, CCompanyTask const & t2) const
    {
        if (t1.m_priority < t2.m_priority)
            return 1;
        else if(t1.m_priority != t2.m_priority)
            return 0;
        else
        {
            if (t1.m_abonents.size() > t2.m_abonents.size())
                return 1;
            else if (t1.m_abonents.size() != t2.m_abonents.size())
                return -1;
            else
                return 0;
        }
    }
};

struct CHandleTaskSorter
{
    bool operator () (CTask_to_handle const & t1, CTask_to_handle const & t2) const
    {
        //return m_cs(*t1.m_task, *t2.m_task);
        int c = m_cs.compar(*t1.m_task, *t2.m_task);
        if (c > 0)
            return true;
        else if (0 != c)
            return false;
        else
            return (t1.m_client.get() > t2.m_client.get());
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
    virtual void CheckIfLeft(CTask_to_handle tsk)  = 0;
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
        //m_strand.wrap

        //m_io_service.post
        m_strand_for_calls.post(boost::bind(&call_executor::CallCompanyTask, this, task));
        m_strand_for_threads.post(boost::bind(&call_executor::setThread, this));
        //setThread();
    }


private:
    //must be called only from m_strand_for_threads
    void  setThread()
    {

        std::unique_ptr<boost::thread>& t2 = t;//.release());
        //create thread if necessary.
        //if (m_io_service.stopped() || !t2.get())// || !t2->joinable() )


//m_strand_for_threads. get_io_service() m_io_service.poll_one();
        if (m_io_service.stopped() ||!t2.get())//
        {
            if (m_io_service.stopped())
                m_io_service.reset();
            // assert(procTC() <= 1);  //boost::this_thread::get_id()breakpoint place

            t2.reset(new boost::thread(boost::bind(&call_executor::Run, this)));
        }
//        else
//            do
//            {
//                if (t2.get() )
//                {
//                    if  ((t2->try_join_for(boost::chrono::milliseconds(0)))
//                            //&& )
//                        )
//                    {
//                        //UNUSED(thisThr)  //thisThr;
//                        break;
//                    } //__attribute__((unused))
//                    bool thisThr = (t2->get_id() == boost::this_thread::get_id());
//                    if (thisThr)
//                        break;
//                    // if (!m_io_service.stopped())
//                    //    break;
//
//                }
//
//                if (m_io_service.stopped())
//                    m_io_service.reset();
//                //assert(procTC() <= 1 );  //boost::this_thread::get_id()breakpoint place
//                // m_strand_for_threads(boost::bind())
//                t2.reset(new boost::thread(boost::bind(&call_executor::Run, this)));
//                // t = t2;
//            }
//            while (0);
        //else


        // if (t2.get())
        //     t.reset(t2.release());
    }
    //  void setThread( std::unique_ptr<boost::thread* pt)

    void Run()
    {
        m_io_service.run();

//        if (m_io_service.stopped())
//        {
//            m_io_service.reset();
//        }
    }
    boost::asio::io_service   m_io_service;

    executor_pool_base & m_pool;
    boost::asio::io_service::strand m_strand_for_threads;
    boost::asio::io_service::strand m_strand_for_calls;

    friend class call_server;
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
    void join(CInteractor_ptr )
    {
        //participant
        using namespace std;
        cout << __FUNCTION__ << " ." << endl;

    }

    void leave(CInteractor_ptr participant)
    {
        using namespace std;
        cout << __FUNCTION__ << " ." << endl;
        PostClearUser(participant);
    }

    void deliver_to_client(CInteractor_ptr client, CServerStatus const& msg) override
    {
        client->deliver(msg);
    }
    boost::asio::io_service& get_io_serv() override
    {
        return m_io_service;
    }

    void make_calls(TInitiaWriteData & msg, CInteractor_ptr client)
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
                read_queue.emplace(
                    CTask_to_handle
                {
                    CCompanyTask_ptr(new CCompanyTask(std::move(msg.front()))),
                    client
                });
                msg.pop_front();
            }
            // emplace(CTask_to_handle{ msg, client});


            task1 = read_queue.top();
            read_queue.pop();
            m_incomplete.insert(task1);
        }

        m_call_executor.Proc(task1);

    }
    void CheckIfLeft(CTask_to_handle tsk) override
    {
        m_io_service.post(boost::bind(&caller_executor_pool::CheckIfLeftAndLoadNext, this, tsk));
    }
private:
    std::set<CTask_to_handle, CHandleTaskSorter> m_incomplete;
    void CheckIfLeftAndLoadNext(CTask_to_handle tsk)
    {
        bool tasks_left = false;
        {
            std::lock_guard<std::mutex> lock(m_queue_m);
            for (CTask_to_handle const& thi : read_queue.GetCont())
            {
                if (thi.m_client == tsk.m_client)//.get()
                {
                    tasks_left= true;
                    break;
                }

            }
            auto ft =  m_incomplete.find(tsk);
            if (ft!=m_incomplete.end())
                m_incomplete.erase(tsk);
            if (!tasks_left)
            {
                for(CTask_to_handle const& th : m_incomplete)
                {
                    if (th.m_client == tsk.m_client)
                    {
                        tasks_left = true;

                        break;
                    }
                }
            }

        }
        CInteractor_ptr  client = tsk.m_client;
        if (!tasks_left)
        {

            client->deliver(CServerStatus(caller_executor_pool::endMessage)); // "All tasks end"

            CServerStatus endStatus (CServerStatus::getLastMark(), CServerStatus::ELastMark);
            assert(endStatus.isLastMark());
            client->deliver(endStatus);
        }
        else
        {
            client->deliver(CServerStatus("Have some your tasks"));


        }
        load_next_task(client);
    }

    void load_next_task(CInteractor_ptr client)
    {

        std::lock_guard<std::mutex> lock(m_queue_m);

        if (!read_queue.empty())
        {


            CTask_to_handle task1 =  read_queue.top();
            read_queue.pop();
            m_incomplete.insert(task1);

            m_call_executor.Proc(task1);
            // CheckIfLeftAndLoadNext(task1.m_client);
        }
        // else
        // {   assert(task




//        recent_msgs_.push_back(msg);
//        while (recent_msgs_.size() > max_recent_msgs)
//            recent_msgs_.pop_front();
//        m_client_app->deliver(msg);
        //  m_call_executor.
//        std::for_each(m_client_app.begin(), m_client_app.end(),
//                      boost::bind(&client_app::deliver, _1, boost::ref(msg)));
    }


    void PostClearUser(CInteractor_ptr tsk)
    {
        m_io_service.post(boost::bind(&caller_executor_pool::ClearUser, this, tsk));
    }

    void ClearUser(CInteractor_ptr intr)
    {

        std::lock_guard<std::mutex> lock(m_queue_m);
        read_queue.eraseAllOfInteractor(intr);

        for(auto itTh  = m_incomplete.begin(); itTh != m_incomplete.end(); )
        {
            if (itTh->m_client == intr)
            {
                //tasks_left = true;
                auto itL = m_incomplete.erase(itTh);
                itTh = itL;

            }
            else
                ++itTh;
        }
    }

private:
//client_app_ptr  m_client_app;
//holds pointers.
//  std::set<CInteractor_ptr> m_client_sessions;
// std::set<CInteractor_weak_ptr> m_client_sessions;
// enum { max_recent_msgs = 100 };

//todo:maybe better to use strand.
    std::mutex m_queue_m;

    typedef    std::priority_queue<CTask_to_handle, std::deque<CTask_to_handle>, CHandleTaskSorter>
    TCont;

    struct UnProtectCont : public TCont
    {

        TCont::container_type const & GetCont() const
        {
            return c;
        }
        struct FunctIsOfIteractor
        {
            CInteractor_ptr intr;//isIteractors
            bool operator ()(CTask_to_handle const& th) const
            {
                return (th.m_client == intr);
            }
        };
        void eraseAllOfInteractor(CInteractor_ptr intr)
        {
            auto & cont = c;//read_queue.GetCont();
            auto initCount = cont.size();
            //at cont.find();
            FunctIsOfIteractor fit = {intr};
            std::remove_if(cont.begin(), cont.end(), fit);
//            for(auto itTh  = cont.begin(); itTh != cont.end(); ++itTh)
//            {
//                if (itTh->m_client == intr)
//                {
//                    //tasks_left = true;
//                    cont.erase(itTh);
//                }
//            }
            if (initCount != cont.size())
                std::make_heap(cont.begin(), cont.end(), comp);//maybe useless

        }
     };
    UnProtectCont read_queue;



// std::set<CCompanyTask, CompaniesSorter>
// std::set<CTask_to_handle,  CHandleTaskSorter>    read_queue;
// server_status_queue recent_msgs_;
    boost::asio::io_service & m_io_service;
    call_executor m_call_executor;
    friend class call_server;
};



#endif // CALL_EXECUTOR_H
