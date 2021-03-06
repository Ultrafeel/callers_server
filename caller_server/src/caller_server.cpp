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
#include <boost/lexical_cast.hpp>
#include "CSettingsReader.h"
#include "../common.h"
#include "../call_executor.h"
using boost::asio::ip::tcp;

//----------------------------------------------------------------------


//----------------------------------------------------------------------

class client_listen_session
    : public CInteractor
   // ,  public boost::enable_shared_from_this<client_listen_session>
{
public:

    boost::shared_ptr<client_listen_session> shared_from_this()
    {
        return static_pointer_cast<client_listen_session >( CInteractor::shared_from_this());
    }
    client_listen_session(boost::asio::io_service& io_service, caller_executor_pool& room)
        : m_socket(io_service),
          m_caller(room)
    {
    }
    ~client_listen_session()
    {}
    tcp::socket& socket()
    {
        return m_socket.socket();
    }

    void leave()
    {

        std::cout<< " Client"<< endpoint()  << "leave " << endl; // <<
        m_caller.leave(shared_from_this());
    }
    void start()
    {
        m_caller.join(shared_from_this());
        std::cout<< " Client "<< endpoint()  << " join " << endl; // <<

        //boost::asio::
        async_read_msg();
    }
    void async_read_msg()
    {
        current_companyTask.reset(new TInitiaWriteData());
        m_socket.async_read(*current_companyTask,
                            // boost::asio::buffer(read_msg_.data(), chat_message::header_length),
                            boost::bind(
                                &client_listen_session::handle_read_company,

                                static_pointer_cast<client_listen_session >( shared_from_this()),
                                boost::asio::placeholders::error));
    }

    void deliver(const CServerStatus& msg) override
    {

        bool write_in_progress = true;
        {
            default_mlock l1(write_msg_mutex);
            write_in_progress = !write_msgs_.empty();
            write_msgs_.push_back(msg);
        }
        if (!write_in_progress)
        {
            default_mlock l2(write_msg_mutex);
            if (!write_msgs_.empty())
            {
                if (write_msgs_.front().isLastMark())
                    cout <<  "   >>No tasks left for:" << endpoint()<<endl;

                std::cout << "  >>" <<__FUNCTION__ << GetTickStr() << ":addr:" << endpoint() <<" " <<  write_msgs_.front().message << std::endl;

                m_socket.async_write(
                    write_msgs_.front(),//  boost::asio::buffer(write_msgs_.front().data(),
                    // write_msgs_.front().length()),
                    boost::bind(&client_listen_session::handle_write, shared_from_this(),
                                boost::asio::placeholders::error));
            }
        }
    }

    void handle_read_company(const boost::system::error_code& error)
    {
        if (!error  )
        {

            m_caller.make_calls(*current_companyTask,CInteractor_ptr( this->shared_from_this()) );
            //async_read_msg();
            std::cout << __FUNCTION__ << " : Companies from " <<  endpoint() << " delivered" << std::endl;
            //m_socket.
            // m_caller.CallCompanyTask()

            // m_caller.CallCompanyTask(read_queue.top());

            //read_queue.pop();
            // async_read_msg();//<recursion
        }
        else
        {
            std::cout << __FUNCTION__ << ": Getting companies from " << endpoint() <<
                      ": error: " << error << ".\n" <<
                      " server will be terminated" << std::endl;
            deliver( CServerStatus(CServerStatus::EServerErrorReadCompanies));
            leave();

            exit(1); // p 3.4
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
//                                    boost::bind(&client_listen_session::handle_read_company, shared_from_this(),
//                                                boost::asio::placeholders::error));
//        }
//        else
//        {
//            m_caller.leave(shared_from_this());
//        }
//    }
private:
    void handle_write(const boost::system::error_code& error)
    {
        if (!error)
        {
            bool isEnd;
            {
                default_mlock l1(write_msg_mutex);
                //!(!write_msgs_.empty() &&
                isEnd =  write_msgs_.front().isLastMark();//caller_executor_pool::endMessage != write_msgs_.front().message);

                write_msgs_.pop_front();

                //loop while write_msgs_ not empty
                if (!write_msgs_.empty())
                {
                    if (isEnd)
                         std::cout << __FUNCTION__ << GetTickStr() << ":addr:" << endpoint() <<": strange " << std::endl;
                    else
                         std::cout << "  <" <<__FUNCTION__ << GetTickStr() << ":addr:" << endpoint() <<" " << std::endl;

                    m_socket.async_write( write_msgs_.front(),//      boost::asio::buffer(write_msgs_.front().data(),
                                          //   write_msgs_.front().length()),
                                          boost::bind(&client_listen_session::handle_write, shared_from_this(),
                                                      boost::asio::placeholders::error));
                }
                else
                {
                    isEnd = false;
                }

            }
            if (isEnd)
            {
                leave();
            }
        }
        else
        {
            std::cout << __FUNCTION__ << GetTickStr() << ":addr:" << endpoint()
            <<": error " << error << std::endl;

            leave();
        }
    }



    // tcp::socket

    serialize_sock::connection m_socket;
    caller_executor_pool& m_caller;
    //TInitiaWriteData
    std::unique_ptr<TInitiaWriteData> current_companyTask;
    // chat_message read_msg_;
    // std::string client_addr;
    boost::asio::ip::tcp::socket::endpoint_type r_endpoint()
    {
        return  m_socket.socket().remote_endpoint();
    }

    std::string m_addr;
    std::string endpoint()
    {

       if ( !m_addr.empty())
            return m_addr;

          boost::system::error_code ec;
            //  std::string s = tmp_ep.to_string(ec);
        std::string s = boost::lexical_cast
                            <std::string>( m_socket.socket().remote_endpoint(ec));
            if (ec)
                  s = "__";

            m_addr = s;

            return s;
    }

    std::mutex write_msg_mutex;

    server_status_queue write_msgs_;
};

typedef boost::shared_ptr<CInteractor> chat_session_ptr;//client_listen_session

//----------------------------------------------------------------------

class call_server
{
public:
    call_server(boost::asio::io_service& io_service,
                const tcp::endpoint& endpoint)
        : io_service_(io_service),
          acceptor_(io_service, endpoint), m_callers_pool(io_service)//caller_executor_pool
    {
        // io_service_
        start_accept();
    }

    void start_accept()
    {
        chat_session_ptr new_session1(new client_listen_session(io_service_, m_callers_pool));
          auto new_session =  static_pointer_cast<client_listen_session >(new_session1);


        acceptor_.async_accept(new_session->socket(),
                               boost::bind(&call_server::handle_accept, this, new_session,
                                           boost::asio::placeholders::error));
    }

    void handle_accept(chat_session_ptr session,
                       const boost::system::error_code& error)
    {
        if (!error)
        {
            static_pointer_cast<client_listen_session >(session)->start();
        }
        else
        {
            std::cout << __FUNCTION__ << ": error " << error << std::endl;
        }

        start_accept();
    }

    void join_caller()
    {
         std::cout << __FUNCTION__ << " " << std::endl;
         assert(!" should not reach here" );
         m_callers_pool.m_call_executor.m_io_service.run();
    }
private:
    boost::asio::io_service& io_service_;
    tcp::acceptor acceptor_;
    caller_executor_pool m_callers_pool;
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
        server.join_caller();
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
