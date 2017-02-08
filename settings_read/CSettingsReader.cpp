/*
 * CSettingsReader.cpp
 *
 *  Created on: Jan 21, 2017
 *      Author: mml
 */
#include <iostream>
#include <algorithm>
#include <functional>

#include "CSettingsReader.h"


CSettingsReader::CSettingsReader(char const* path):m_path(path) {
	// TODO Auto-generated constructor stub
	//printf(path);atoi(path);
	//CSettingsReader::CValue v;
}

 CSettingsReader::~CSettingsReader() {
	 // TODO Auto-generated destructor stub
 }

using namespace std;
bool isspaceM(char ch)
{
	return std::isspace(string::traits_type::to_int_type(ch));
}

bool isNotSpaceM(char ch)
{
	return  !isspaceM(ch);
}
string CSettingsReader::trimWhitesp(string const & str)
{
	//function<bool >  f =  isspaceM ;
	string::const_iterator b = std::find_if(str.begin(), str.end(),  isNotSpaceM);//std::unary_negate( f)
	auto e = std::find_if(b, str.end(), isspaceM );
	return string(b, e);
}
//const char  CSettingsReader::delimiter1 = '=';
bool CSettingsReader::OpenAndRead()
{
    m_file.open(m_path.c_str());
    if(!m_file.is_open())
    {
        cout << "no settings file" << endl;
        return false;
    }
    std::string str;
    while (m_file.good())
    {
        getline(m_file,str);
        auto split1 =str.find(delimiter1);
        if (split1 != string::npos)
        {
            auto itDelim =  str.begin() + split1;
            string key0 (str.begin(), itDelim);
            string key = trimWhitesp(key0);
            if (itDelim != str.end())
                ++itDelim;
            else
                return false;
            string value0( itDelim, str.end());
            string value = trimWhitesp(value0);
            m_parsed.insert(make_pair (key, value));
            char * pEnd = 0;
            auto i = strtol(value.c_str(), &pEnd, 10);
            if (pEnd && pEnd > value.c_str())
            {
                m_parsedInt.insert( make_pair( key, i));
            }
        }
    }
    return true;
}
