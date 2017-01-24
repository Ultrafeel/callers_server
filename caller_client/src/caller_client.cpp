//============================================================================
// Name        : caller_client.cpp
// Author      : mml
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <CSettingsReader.h>
using namespace boost;
using boost::asio::ip::tcp;

using namespace std;

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	char const * cfgP = "config_cl.cfg";
	CSettingsReader cfg(cfgP);
	bool res = cfg.OpenAndRead();
    if(!res) { //cfg.m_file.rdbuf()->
        std::cout << " cfg wrong .  "   << '\n';
        return 1;
    }
    auto itPort = cfg.m_parsedInt.find("port");//[;
    auto itPortStr = cfg.m_parsed.find("port");//[;
    //auto search = example(2);
    if(itPort == cfg.m_parsedInt.end()) { //cfg.m_file.rdbuf()->
        std::cout << " cfg wrong . Port not Found in : " << cfgP << " "   << '\n';
        return 1;
    }

   // cfg.m_parsedInt.find("port")
     auto itServAddr = cfg.m_parsed.find("server_addr");//[;
    //auto search = example(2);
    if(itServAddr == cfg.m_parsed.end()) { //cfg.m_file.rdbuf()->
        std::cout << " cfg wrong . server_addr not Found in : " << cfgP << " "   << '\n';
        return 1;
    }
    cout << " cfg  Server  to use   : " <<  itServAddr->second << " "   << '\n';

    cout << " cfg  Port  to use   : " <<  itPort->second << " (" << itPortStr->second  << ")\n";

    asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(itServAddr->second, itPortStr->second);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);


    return 0;
}
