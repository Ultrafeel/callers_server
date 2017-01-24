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

    asio::io_service io_service;



	return 0;
}
