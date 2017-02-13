//============================================================================
// Name        : caller_client.cpp
// Author      : mml
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
//
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
//#include <boost/array.hpp>
#include <CSettingsReader.h>
#include <cserverstatus.h>
#include <ccompanytask.h>
#include <connection.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/deque.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "../ccompaniesreader.h"
using boost::asio::ip::tcp;

typedef std::deque<CCompanyTask> companies_queue;

class Caller_client
{
public:
    Caller_client(boost::asio::io_service& io_service,
                  tcp::resolver::iterator endpoint_iterator)
        : io_service_(io_service),
          socket_(io_service)
    {
        boost::asio::async_connect(socket_.socket(), endpoint_iterator,
                                   boost::bind(&Caller_client::handle_connect, this,
                                               boost::asio::placeholders::error));
    }

    void write(const TInitiaWriteData& msgs)
    {
        io_service_.post(boost::bind(&Caller_client::do_write, this, boost::ref(msgs)));
    }

    void close()
    {
        io_service_.post(boost::bind(&Caller_client::do_close, this));
    }


    std::string m_addr;

    std::string local_endpoint_str()
    {
       if ( !m_addr.empty())
            return m_addr;

        boost::system::error_code ec ;
        //  std::string s = tmp_ep.to_string(ec);
        std::stringstream s;  //=boost::lexical_cast   <std::string>(
        auto lep = socket_.socket().local_endpoint(ec);

        if (ec)
            s << "__";
        else
            s << lep;
        m_addr = s.str();

        return m_addr;
    }

private:

    void handle_connect(const boost::system::error_code& error)
    {
        if (!error)
        {
            std::cout << " Connected "  << local_endpoint_str() <<  std::endl;
            write(m_companiesToWrite);
        }
        else
        {
            std::cout << "Cannot connect to server. (" << __FUNCTION__ << ": error " << error << ")" <<std::endl;
            exit(EXIT_FAILURE); // p2.1
        }
    }

    void handle_read_response(const boost::system::error_code& error)
    {
        if (!error)// && read_msg_.decode_header())
        {
            using namespace std;
            if (read_msg_.isLastMark())
            {
                cout << "server: have not any of your company task: " <<  read_msg_.message << endl;
                close();
            }
            else if (read_msg_.m_enum == CServerStatus::EServerErrorReadCompanies)
            {
                cout << "server: failed: " <<  read_msg_.message << endl;
                cout << "Cannot continue: program will be terminated. " <<  endl;
                close();
                exit(1); //p 2.4
            }
            else
            {
                cout << "currently : " <<  read_msg_.message << endl;
                socket_.async_read(
                    read_msg_, //boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
                    boost::bind(&Caller_client::handle_read_response, this,
                                boost::asio::placeholders::error));
            }
        }
        else
        {
            std::cout << __FUNCTION__ << ": error " << error << std::endl;

            do_close();
        }
    }

//    void handle_read_body(const boost::system::error_code& error)
//    {
//        if (!error)
//        {
//            std::cout.write(read_msg_.body(), read_msg_.body_length());
//            std::cout << "\n";
//               socket_.async_read(
//                                    read_msg_,//boost::asio::buffer(read_msg_.data(), chat_message::header_length),
//                                    boost::bind(&Caller_client::handle_read_response, this,
//                                                boost::asio::placeholders::error));
//        }
//        else
//        {
//            do_close();
//        }
//    }

    void do_write(TInitiaWriteData const& msg)
    {
        if (!socket_.socket().is_open())
        {
            assert(!"must never to reach here");
            std::cout<< " Socket is not opened." << __FUNCTION__ << std::endl;
            exit(EXIT_FAILURE);
            return;
        }
        socket_.async_write(
            msg,// boost::asio::buffer(write_msgs_.front().data(),
            // write_msgs_.front().length()),
            boost::bind(&Caller_client::handle_write, this,
                        boost::asio::placeholders::error));
    }
    void handle_write(const boost::system::error_code& error)
    {
        if (!error)
        {

            std::cout << __FUNCTION__ << ": finished transmitting Companies. " << std::endl;

            std::cout << __FUNCTION__ << ": start mode 2: listen server response. " << std::endl;

            socket_.async_read(
                read_msg_, //boost::asio::buffer(read_msg_.data(), chat_message::header_length),
                boost::bind(&Caller_client::handle_read_response, this,
                            boost::asio::placeholders::error));



        }
        else
        {
            std::cout << __FUNCTION__ << ": error " << error << std::endl;

            do_close();
        }
    }

    void do_close()
    {
        std::cout << GetTickStr()<< " client closing " << local_endpoint_str() << std::endl;
        socket_.socket().close();
    }
public:
    void SetData(TInitiaWriteData &data)
    {
        m_companiesToWrite.swap(data);
    }
private:
    TInitiaWriteData m_companiesToWrite;
    boost::asio::io_service& io_service_;
    //tcp::socket
    serialize_sock::connection socket_;
    CServerStatus read_msg_;
    companies_queue write_msgs_;
};

using namespace std;
int main(int argc, char* argv[])
{

    char const * compTasks = "companies_test.info";

    // Check command line arguments.
    if (argc > 2)
    {
        std::cerr << "Usage: caller_client \"<companies file name>\"" << std::endl;
        return 1;
    }
    else if (argc == 2)
    {
        compTasks = argv[1];
    }

    cout << "!!!Hello Client!!! Used companies file: \"" << compTasks << "\"" << endl; // prints !!!Hello World!!!
    char const * cfgP = "config_cl.cfg";
    CSettingsReader cfg(cfgP);
    bool res = cfg.OpenAndRead();
    if(!res)   //cfg.m_file.rdbuf()->
    {
        std::cout << " cfg " << cfgP <<" wrong .  "   << '\n';
        return 1;
    }
    auto itPort = cfg.m_parsedInt.find("port");//[;
    auto itPortStr = cfg.m_parsed.find("port");//[;
    //auto search = example(2);
    if(itPort == cfg.m_parsedInt.end())   //cfg.m_file.rdbuf()->
    {
        std::cout << " cfg wrong . Port not Found in : " << cfgP << " "   << '\n';
        return 1;
    }
///
    // cfg.m_parsedInt.find("port")
    auto itServAddr = cfg.m_parsed.find("server_addr");//[;
    //auto search = example(2);
    if(itServAddr == cfg.m_parsed.end())   //cfg.m_file.rdbuf()->
    {
        std::cout << " cfg wrong . server_addr not Found in : " << cfgP << " "   << '\n';
        return 1;
    }
    cout << " cfg  Server  to use   : " <<  itServAddr->second << " "   << '\n';

    cout << " cfg  Port  to use   : " <<  itPort->second << " (" << itPortStr->second  << ")\n";


    CCompaniesReader cr(compTasks);
    if (!cr.OpenAndReadTasks())
    {
        cout << " failed to parse " << compTasks << endl;
        return 1;
    }

    try
    {

        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(itServAddr->second, itPortStr->second);
        // std::getchar();
        tcp::resolver::iterator iterator = resolver.resolve(query);

        Caller_client c(io_service, iterator);
        c.SetData(cr.m_tasks);

        boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));

        // char line[CCompanyTask::max_message_length + 1];
        // while (std::cin.getline(line, CCompanyTask::max_message_length + 1))
//        for ( CCompanyTask & msg : cr.m_tasks)
//        {
        // using namespace std; // For strlen and memcpy.
        //CCompanyTask msg=  TCompanyTask { line };
//            msg.body_length(strlen(line));
//            memcpy(msg.body(), line, msg.body_length());
//            msg.encode_header();
//        }

        //c.close();
        t.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
        std::cerr << "Connection to caller_server failed: " << e.what() << std::endl;
        return 1;
    }
    cout << " execution finished: enter any char"<< endl;
    std::getchar();
    return 0;
}
