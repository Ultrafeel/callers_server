//============================================================================
// Name        : caller_server.cpp
// Author      : mml
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//#include <ctime>
#include <iostream>
#include <string>
//#include <boost/array.hpp>


#include <algorithm>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <set>
#include <queue>
#include <mutex>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "ccompanytask.h"
#include "cserverstatus.h"
#include "connection.hpp" // Must come before boost/serialization headers.
#include <boost/serialization/vector.hpp>
#include <boost/serialization/deque.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "CSettingsReader.h"
#include "../call_executor.h"
using boost::asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<CServerStatus> server_status_queue;

//----------------------------------------------------------------------

class CInteractor
{
public:
    virtual ~CInteractor() {}
    virtual void deliver(const CServerStatus& msg) = 0;
};

typedef boost::shared_ptr<CInteractor> CInteractor_ptr;

//----------------------------------------------------------------------
using namespace std;
char  const * endMessage = "\n end";
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

struct CTask_to_handle
{
    CCompanyTask m_task;
    CInteractor_ptr m_client;
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



class caller_executor_pool : public boost::noncopyable
{

    caller_executor_pool( boost::asio::io_service & io_service_):
        m_io_service(io_service_)
    {

    }
    friend class call_server;
public:


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
    void CallCompanyTask(CTask_to_handle const& th)
    {
        CInteractor_ptr  CInteractor = th.m_client;
        if (CInteractor.get())
            return;

        CCompanyTask  const& ct = th.m_task;
        cout << "task ";
        cout <<ct.m_comp_name << endl ;
        CInteractor->deliver(CServerStatus( "compon name " +  ct.m_comp_name));
        size_t iu = 1;
        for (CAbonent const& us : ct.m_abonents)
        {
            cout << ++iu << " abonent " << us.m_name << endl;

            CInteractor->deliver(CServerStatus(us.m_name ));
        }
        bool tasks_left = false;
        CInteractor->deliver(CServerStatus(endMessage));
        for (CTask_to_handle const& thi : read_queue)
        {
            if (thi.m_client.get() == CInteractor.get())
                tasks_left= true;
        }
        if (tasks_left)
            CInteractor->deliver(CServerStatus("All tasks end"));
        else
            CInteractor->deliver(CServerStatus("Have some your tasks"));

    }

    void deliver(CCompanyTask&& msg, CInteractor_ptr client)
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

    //std::priority_queue<CCompanyTask, std::list<CCompanyTask>, CompaniesSorter>
   // std::set<CCompanyTask, CompaniesSorter>
    std::set<CTask_to_handle,  CHandleTaskSorter>    read_queue;
    // server_status_queue recent_msgs_;
    boost::asio::io_service & m_io_service;
};

//----------------------------------------------------------------------

class client_listen_session
    : public CInteractor,
      public boost::enable_shared_from_this<client_listen_session>
{
public:
    client_listen_session(boost::asio::io_service& io_service, caller_executor_pool& room)
        : m_socket(io_service),
          m_caller(room)
    {
    }

    tcp::socket& socket()
    {
        return m_socket.socket();
    }

    void start()
    {
        m_caller.join(shared_from_this());
        //boost::asio::
        async_read_msg();
    }
    void async_read_msg()
    {
        current_companyTask.reset(new CCompanyTask());
        m_socket.async_read(*current_companyTask,
                            // boost::asio::buffer(read_msg_.data(), chat_message::header_length),
                            boost::bind(
                                &client_listen_session::handle_read_header, shared_from_this(),
                                boost::asio::placeholders::error));
    }

    void deliver(const CServerStatus& msg)
    {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress)
        {
            m_socket.async_write(
                write_msgs_.front(),//  boost::asio::buffer(write_msgs_.front().data(),
                // write_msgs_.front().length()),
                boost::bind(&client_listen_session::handle_write, shared_from_this(),
                            boost::asio::placeholders::error));
        }
    }

    void handle_read_header(const boost::system::error_code& error)
    {
        if (!error  )
        {

            {
                 m_caller.deliver(*current_companyTask.release() ,CInteractor_ptr( this->shared_from_this()) );

            }
            //m_socket.
           // m_caller.CallCompanyTask()
            m_caller.CallCompanyTask(read_queue.top());
            //read_queue.pop();
           // async_read_msg();//<recursion
        }
        else
        {
            m_caller.leave(shared_from_this());
        }
    }
//
//    void handle_read_body(const boost::system::error_code& error)
//    {
//        if (!error)
//        {
//            m_caller.deliver(read_msg_);
//
//            socket_.async_read( task //boost::asio::buffer(read_msg_.data(), chat_message::header_length),
//                                    boost::bind(&client_listen_session::handle_read_header, shared_from_this(),
//                                                boost::asio::placeholders::error));
//        }
//        else
//        {
//            m_caller.leave(shared_from_this());
//        }
//    }

    void handle_write(const boost::system::error_code& error)
    {
        if (!error)
        {
            bool isEnd = (!write_msgs_.empty() && endMessage != write_msgs_.front().message);

            write_msgs_.pop_front();
            //loop while write_msgs_ not empty
            if (!write_msgs_.empty())
            {
                m_socket.async_write( write_msgs_.front(),//      boost::asio::buffer(write_msgs_.front().data(),
                                      //   write_msgs_.front().length()),
                                      boost::bind(&client_listen_session::handle_write, shared_from_this(),
                                                  boost::asio::placeholders::error));
            }

            if (isEnd)
                m_caller.leave(shared_from_this());
        }
        else
        {
            m_caller.leave(shared_from_this());
        }
    }

private:

    // tcp::socket

    serialize_sock::connection m_socket;
    caller_executor_pool& m_caller;
    std::unique_ptr<CCompanyTask> current_companyTask;
    // chat_message read_msg_;
    server_status_queue write_msgs_;
};

typedef boost::shared_ptr<client_listen_session> chat_session_ptr;

//----------------------------------------------------------------------

class call_server
{
public:
    call_server(boost::asio::io_service& io_service,
                const tcp::endpoint& endpoint)
        : io_service_(io_service),
          acceptor_(io_service, endpoint)
    {
        start_accept();
    }

    void start_accept()
    {
        chat_session_ptr new_session(new client_listen_session(io_service_, m_caller));
        acceptor_.async_accept(new_session->socket(),
                               boost::bind(&call_server::handle_accept, this, new_session,
                                           boost::asio::placeholders::error));
    }

    void handle_accept(chat_session_ptr session,
                       const boost::system::error_code& error)
    {
        if (!error)
        {
            session->start();
        }

        start_accept();
    }

private:
    boost::asio::io_service& io_service_;
    tcp::acceptor acceptor_;
    caller_executor_pool m_caller;
};

//typedef boost::shared_ptr<chat_server> chat_server_ptr;
//typedef std::list<chat_server_ptr> chat_server_list;
using namespace std;



int main()
{
    cout << "!!!Hello Server!!" << endl; // prints !!!Hello Server!!

    char const * cfgP = "config.cfg";
    CSettingsReader cfg(cfgP);
    bool res = cfg.OpenAndRead();
    if(!res)   //cfg.m_file.rdbuf()->
    {
        std::cout << " cfg wrong .  "   << '\n';
        return 1;
    }
    auto itPort = cfg.m_parsedInt.find("port");//[;
    //auto search = example(2);
    if(itPort == cfg.m_parsedInt.end())   //cfg.m_file.rdbuf()->
    {
        std::cout << " cfg wrong . Port not Found in : " << cfgP << " "   << '\n';
        return 1;
    }
    cout << " cfg  Port  to use   : " <<  itPort->second << " "   << '\n';


    try
    {
        boost::asio::io_service io_service;
        tcp::endpoint endpoint(tcp::v4(), itPort->second);
        call_server server(io_service, endpoint);
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
//boost::archive::text_iarchive_impl<boost::archive::text_iarchive> ar;
    return 0;
}
//boost::archive::text_iarchive

///usr/include/boost/archive/text_iarchive.hpp
//boost::archive::text_iarchive_impl
