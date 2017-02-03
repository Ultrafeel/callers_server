#ifndef CALL_EXECUTOR_H
#define CALL_EXECUTOR_H
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <set>
#include <assert.h>
#include <mutex>
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
        return m_cs(t1.m_task, t2.m_task);
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
};
class call_executor
    : //public CInteractor,
      public boost::enable_shared_from_this<call_executor>
{
public:
    call_executor(executor_pool_base & pool);
    virtual ~call_executor();



    void Proc(CTask_to_handle & task)
    {
        if (boost::asio::io_service *  pIo_service = m_pio_service)
        {
            pIo_service->post(boost::bind(&call_executor::CallCompanyTask, this, task));
        }
        else
        {
            assert(0);
        }
        //m_io_service.post()
    }

    boost::asio::io_service *  m_pio_service;
    executor_pool_base & m_pool;
protected:

private:
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

    static char  const * endMessage ;//= "\n end";
    void join(CInteractor_ptr participant)
    {
        // m_client_app = (participant);
//        std::for_each(recent_msgs_.begin(), recent_msgs_.end(),
//                      boost::bind(&client_app::deliver, participant, _1));
    }

    void leave(CInteractor_ptr participant)
    {
        // m_client_app .reset(); //erase(participant);
    }

   void deliver_to_client(CInteractor_ptr client, CServerStatus const& msg) override
   {
       client->deliver(msg);
   }

    void deliver(CCompanyTask const& msg, CInteractor_ptr client)
    {

        CTask_to_handle task1;
        {
            std::lock_guard<std::mutex> lock(m_queue_m);
            // read_queue.emplace(*current_companyTask.release());

            //CallCompanyTask(msg);
            read_queue.emplace(CTask_to_handle{ msg, client});
            auto top = read_queue.begin();
            task1 = *top;
            read_queue.erase(top);
        }
        m_call_executor.Proc(task1);

        bool tasks_left = false;
        for (CTask_to_handle const& thi : read_queue)
        {
            if (thi.m_client.get() == client.get())
                tasks_left= true;
        }
        if (tasks_left)
            client->deliver(CServerStatus(caller_executor_pool::endMessage)); // "All tasks end"
        else
            client->deliver(CServerStatus("Have some your tasks"));

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
    std::set<CInteractor_ptr> m_client_app;
    enum { max_recent_msgs = 100 };
    std::mutex m_queue_m;

 //std::priority_queue<CCompanyTask, std::list<CCompanyTask>, CompaniesSorter>
    // std::set<CCompanyTask, CompaniesSorter>
    std::set<CTask_to_handle,  CHandleTaskSorter>    read_queue;
    // server_status_queue recent_msgs_;
    boost::asio::io_service & m_io_service;
    call_executor m_call_executor;
};



#endif // CALL_EXECUTOR_H
