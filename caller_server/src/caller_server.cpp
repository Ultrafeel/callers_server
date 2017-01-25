//============================================================================
// Name        : caller_server.cpp
// Author      : mml
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <ctime>
#include <iostream>
#include <string>
//#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "CSettingsReader.h"
using namespace std;
using namespace boost;
using boost::asio::ip::tcp;


std::string make_daytime_string()
{
  return "wow!";
}

class tcp_connection
  : public boost::enable_shared_from_this<tcp_connection>
{
public:
  typedef boost::shared_ptr<tcp_connection> pointer;

  static pointer create(boost::asio::io_service& io_service)
  {
    return pointer(new tcp_connection(io_service));
  }

  tcp::socket& socket()
  {
    return socket_;
  }

  void start()
  {
    std::vector<char> buff;
    boost::asio::async_read(socket_, boost::asio::buffer(buff),
        boost::bind(&tcp_connection::handle_read, shared_from_this(),
          boost::asio::placeholders::error
          ,boost::asio::placeholders::bytes_transferred()
                    ));
    message_ = make_daytime_string();

    boost::asio::async_write(socket_, boost::asio::buffer(message_),
        boost::bind(&tcp_connection::handle_write, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }

private:
  tcp_connection(boost::asio::io_service& io_service)
    : socket_(io_service)
  {
  }

  void handle_write(const boost::system::error_code& /*error*/,
      size_t /*bytes_transferred*/)
  {
  }

  void handle_read(const boost::system::error_code& /*error*/
                 ,
      size_t  /* bytes_transferred*/)
  {
  }
  tcp::socket socket_;
  std::string message_;
};

class tcp_server
{
public:
  tcp_server(boost::asio::io_service& io_service, unsigned short port_num)
    : acceptor_(io_service, tcp::endpoint(tcp::v4(), port_num))
  {
    start_accept();
  }

private:
  void start_accept()
  {
    tcp_connection::pointer new_connection =
      tcp_connection::create(acceptor_.get_io_service());

    acceptor_.async_accept(new_connection->socket(),
        boost::bind(&tcp_server::handle_accept, this, new_connection,
          boost::asio::placeholders::error));
  }

  void handle_accept(tcp_connection::pointer new_connection,
      const boost::system::error_code& error)
  {
    if (!error)
    {
      new_connection->start();
    }

    start_accept();
  }

  tcp::acceptor acceptor_;
};



int main() {
	cout << "!!!Hello Server!!" << endl; // prints !!!Hello Server!!

	char const * cfgP = "config.cfg";
	CSettingsReader cfg(cfgP);
	bool res = cfg.OpenAndRead();
    if(!res) { //cfg.m_file.rdbuf()->
        std::cout << " cfg wrong .  "   << '\n';
        return 1;
    }
    auto itPort = cfg.m_parsedInt.find("port");//[;
    //auto search = example(2);
    if(itPort == cfg.m_parsedInt.end()) { //cfg.m_file.rdbuf()->
        std::cout << " cfg wrong . Port not Found in : " << cfgP << " "   << '\n';
        return 1;
    }
    cout << " cfg  Port  to use   : " <<  itPort->second << " "   << '\n';


  try
  {
    boost::asio::io_service io_service;
    tcp_server server(io_service, itPort->second );
    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

	return 0;
}
