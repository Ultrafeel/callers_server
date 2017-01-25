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
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "chat_message.hpp"
#include "CSettingsReader.h"
#include "ccompanytask.h"
using boost::asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<chat_message> chat_message_queue;

//----------------------------------------------------------------------

class client_app
{
public:
    virtual ~client_app() {}
    virtual void deliver(const chat_message& msg) = 0;
};

typedef boost::shared_ptr<client_app> client_app_ptr;

//----------------------------------------------------------------------

class caller_executor : public boost::noncopyable
{

    caller_executor()
    {

    }
    friend class call_server;
public:

    void join(client_app_ptr participant)
    {
        m_client_app = (participant);
        std::for_each(recent_msgs_.begin(), recent_msgs_.end(),
                      boost::bind(&client_app::deliver, participant, _1));
    }

    void leave(client_app_ptr participant)
    {
        m_client_app.erase(participant);
    }
    void CallCompanyTask(CCompanyTask const& ct)
    {

            for (auto& us : ct.m_users)
            {

            }
    }

    void deliver(const chat_message& msg)
    {
        recent_msgs_.push_back(msg);
        while (recent_msgs_.size() > max_recent_msgs)
            recent_msgs_.pop_front();
        m_client_app->deliver(msg);
//        std::for_each(m_client_app.begin(), m_client_app.end(),
//                      boost::bind(&client_app::deliver, _1, boost::ref(msg)));
    }

private:
    client_app_ptr  m_client_app;
    //std::set<client_app_ptr> m_client_app;
    enum { max_recent_msgs = 100 };
    chat_message_queue recent_msgs_;
};

//----------------------------------------------------------------------

class client_listen_session
    : public client_app,
      public boost::enable_shared_from_this<client_listen_session>
{
public:
    client_listen_session(boost::asio::io_service& io_service, caller_executor& room)
        : socket_(io_service),
          m_caller(room)
    {
    }

    tcp::socket& socket()
    {
        return socket_;
    }

    void start()
    {
        m_caller.join(shared_from_this());
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.data(), chat_message::header_length),
                                boost::bind(
                                    &client_listen_session::handle_read_header, shared_from_this(),
                                    boost::asio::placeholders::error));
    }

    void deliver(const chat_message& msg)
    {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress)
        {
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(write_msgs_.front().data(),
                                             write_msgs_.front().length()),
                                     boost::bind(&client_listen_session::handle_write, shared_from_this(),
                                                 boost::asio::placeholders::error));
        }
    }

    void handle_read_header(const boost::system::error_code& error)
    {
        if (!error && read_msg_.decode_header())
        {
            boost::asio::async_read(socket_,
                                    boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
                                    boost::bind(&client_listen_session::handle_read_body, shared_from_this(),
                                                boost::asio::placeholders::error));
        }
        else
        {
            m_caller.leave(shared_from_this());
        }
    }

    void handle_read_body(const boost::system::error_code& error)
    {
        if (!error)
        {
            m_caller.deliver(read_msg_);
            boost::asio::async_read(socket_,
                                    boost::asio::buffer(read_msg_.data(), chat_message::header_length),
                                    boost::bind(&client_listen_session::handle_read_header, shared_from_this(),
                                                boost::asio::placeholders::error));
        }
        else
        {
            m_caller.leave(shared_from_this());
        }
    }

    void handle_write(const boost::system::error_code& error)
    {
        if (!error)
        {
            write_msgs_.pop_front();
            if (!write_msgs_.empty())
            {
                boost::asio::async_write(socket_,
                                         boost::asio::buffer(write_msgs_.front().data(),
                                                 write_msgs_.front().length()),
                                         boost::bind(&client_listen_session::handle_write, shared_from_this(),
                                                     boost::asio::placeholders::error));
            }
        }
        else
        {
            m_caller.leave(shared_from_this());
        }
    }

private:
    tcp::socket socket_;
    caller_executor& m_caller;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
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
    caller_executor m_caller;
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

    return 0;
}
