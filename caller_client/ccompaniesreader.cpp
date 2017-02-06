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


bool CCompaniesReader::OpenAndReadTasks()
{
   static const auto path = "companies_test.info";
    using namespace boost::property_tree;
    boost::property_tree::ptree pt;
    if (m_path.empty())
        m_path = path;
    try
    {
        boost::property_tree::info_parser::read_info(m_path, pt);// "config.ini",
    }
    catch(std::exception &ex)
    {
        std::cerr<< ex.what() << std::endl;
        return false;
    }
    auto p =  pt.get_optional<std::string>("Task.Users.");
    if (p)
        std::cout << *p << std::endl;
    auto companies = pt.get_child_optional("companies"); //""<std::string>
    if (companies)
    {
        for (ptree::const_iterator itComp = companies->begin(); itComp != companies->end(); ++itComp)
        {
            m_tasks.resize(m_tasks.size() + 1);
            CCompanyTask& company = m_tasks.back();

            // auto p2  =  pt.get_child_optional("");//<std::string>

            //   get("---")    for (ptree::const_iterator  nid= p2->begin(); nid != p2->end(); )
            company.m_comp_name = itComp->first;
            std::cout << " comp name: " << itComp->first << std::endl;
            company.m_priority = itComp->second.get("priority", 0);
            std::cout << " comp priority: " <<  company.m_priority  << std::endl;
            company.m_advertise = itComp->second.get("adv", "--");
            std::cout << " comp adv: " << company.m_advertise << std::endl;
            auto users = itComp->second.get_child_optional("Users"); //""<std::string>
            if (users)
            {
                for (ptree::const_iterator itUser = users->begin(); itUser != users->end(); ++itUser)
                {
                    company.m_abonents.resize(company.m_abonents.size() + 1);
                    CAbonent & abon =  company.m_abonents.back();
                    abon.m_name = itUser->first;
                    abon.m_number = itUser->second.get_value(" no num!!") ;
                    std::cout << " user name: " <<  abon.m_name << std::endl;//second.get("adv", "-no adv!-")

                    std::cout << " user num: " << abon.m_number << std::endl;
                }

            }
        }
    }
    return true;
}


