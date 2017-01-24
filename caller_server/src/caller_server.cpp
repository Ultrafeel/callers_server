//============================================================================
// Name        : caller_server.cpp
// Author      : mml
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "CSettingsReader.h"
using namespace std;
using namespace boost;
using boost::asio::ip::tcp;

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;

  return "wow!";
}



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

    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), itPort->second));

    for (;;)
    {
      tcp::socket socket(io_service);
      acceptor.accept(socket);

      std::string message = make_daytime_string();

      boost::system::error_code ignored_error;
      boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }


	return 0;
}
