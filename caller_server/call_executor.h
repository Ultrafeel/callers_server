#ifndef CALL_EXECUTOR_H
#define CALL_EXECUTOR_H
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <set>
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
class call_executor
    : //public CInteractor,
      public boost::enable_shared_from_this<call_executor>
{
     public:
        call_executor();
        virtual ~call_executor();



        boost::asio::io_service *  m_pio_service;
        caller_executor_pool * m_pool;
    protected:

    private:
         void CallCompanyTask(CTask_to_handle const& th);

};


class caller_executor_pool : public boost::noncopyable
{

    caller_executor_pool( boost::asio::io_service & io_service_):
      m_call_executor() , m_io_service(io_service_)
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


    void deliver(CCompanyTask const& msg, CInteractor_ptr client)
    {

       {
            std::lock_guard<std::mutex> lock(m_queue_m);
               // read_queue.emplace(*current_companyTask.release());

        //CallCompanyTask(msg);
        read_queue.emplace(CTask_to_handle{ msg, client});
       }
//        recent_msgs_.push_back(msg);
//        while (recent_msgs_.size() > max_recent_msgs)
//            recent_msgs_.pop_front();
//        m_client_app->deliver(msg);
        m_io_service.post(boost::bind(& caller_executor_pool::Proc, this));
//        std::for_each(m_client_app.begin(), m_client_app.end(),
//                      boost::bind(&client_app::deliver, _1, boost::ref(msg)));
    }

    void Proc()
    {

        //m_io_service.post()
    }
private:
    //client_app_ptr  m_client_app;
    std::set<CInteractor_ptr> m_client_app;
    enum { max_recent_msgs = 100 };
    std::mutex m_queue_m;

    call_executor m_call_executor;
    //std::priority_queue<CCompanyTask, std::list<CCompanyTask>, CompaniesSorter>
   // std::set<CCompanyTask, CompaniesSorter>
    std::set<CTask_to_handle,  CHandleTaskSorter>    read_queue;
    // server_status_queue recent_msgs_;
    boost::asio::io_service & m_io_service;
};



#endif // CALL_EXECUTOR_H
