#include "ccompaniesreader.h"
#include <iostream>
#include <algorithm>
#include <functional>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>


CCompaniesReader::CCompaniesReader(char const* path):CSettingsReader(path)
{
    //ctor
}

CCompaniesReader::~CCompaniesReader()
{
    //dtor
}

enum ParserState
{
    Empty,
    ReadTask,
    ReadUser
};
char const * keywordTask = "[Task]";
char const * KeywordUser = "[User]";
using namespace std;
bool CCompaniesReader::OpenAndReadTasks2()
{
    boost::property_tree::ptree pt;
    boost::property_tree::info_parser::read_info(m_path, pt);// "config.ini",
    auto p =  pt.get<std::string>("Task.Users.");
    std::cout << p << std::endl;
    p =  pt.get<std::string>("Task.Users.");
      std::cout << p << std::endl;

    std::cout << pt.get<std::string>("Section1.Value2") << std::endl;
    return true;
}
bool CCompaniesReader::OpenAndReadTasks()
{


    m_file.open(m_path.c_str());
    if(!m_file.is_open())
    {
        cout << "no settings file" << endl;
        return false;
    }
    std::string str;
    ParserState state = Empty;
    while (m_file.good())
    {
        getline(m_file,str);

        string key0 = trimWhitesp(str);
        if (keywordTask      == key0)
        {
            state = ReadTask;
        }
        if (KeywordUser     == key0)
        {
            state = ReadUser;
        }
        else
            return false;


        switch (state)
        {
        case Empty:
        {
            string key = trimWhitesp(str);
            if (keywordTask      == key)
                state = ReadTask;
            else
                return false;



        }
        case ReadTask:
//go through
        case ReadUser:
        {

            auto split1 =str.find(delimiter1);
            if (split1 == string::npos)
            {
                continue;
            }
                auto itDelim =  str.begin() + split1;
                string key0 (str.begin(), itDelim);
                string key = trimWhitesp(key0);
           CCompanyTask & comp = m_tasks.back();
            if (ReadTask == state)
            {
                 if (itDelim != str.end())
                    ++itDelim;
                else
                    return false;

                string value( itDelim, str.end());
                char * pEnd = 0;
                string valueT = trimWhitesp(value);


                if ("Name"      == key)
                    comp.m_comp_name = value;
                else if ("Priority" == key)
                {
                    auto i = strtol(value.c_str(), &pEnd, 10);
                    if (pEnd && pEnd > value.c_str())
                    {
                        comp.m_priority = i; //m_parsedInt.insert( make_pair( key, i));
                    }
                    else
                        return false;

                }
                else if ("Advertise" == key)
                    comp.m_advertise = value;
            m_parsed.insert(make_pair (key, value));
            auto i = strtol(value.c_str(), &pEnd, 10);
            if (pEnd && pEnd > value.c_str())
            {
                m_parsedInt.insert( make_pair( key, i));
            }
                   }
            else
            {
                comp.m_abonents.resize(comp.m_abonents.size() + 1);
               // CAbonent & abon = comp.m_abonents.back();
//            if (keywordUser      == key)
//                                      state = ReadUser;
               // if ("Name" == key)


                }

        }
        }
        break;
    }
    return true;
}


