#include "ccompanytask.h"
#include  <cstring>
#include  <string>
#include <algorithm>
#include  <string.h>
#include <chrono>
CCompanyTask::CCompanyTask()//: m_usCount(0)
{
    //ctor
}

CCompanyTask::~CCompanyTask()
{
    //dtor
}
//void CCompanyTask::decode(char const * data, size_t length)
//{
//    auto l = strnlen_sm(data, length);
//    m_comp_name =  std::string(data, l); //std::min(l, length));
//    auto l2 = m_comp_name.length();
//    if (l2 < length)
//        m_usCount = data[l2];
//    auto currPos = l2 + sizeof(char);
//    for (auto i = 0; (i < m_usCount) && (currPos <= length); ++i)
//    {
//        auto pCurr = data + currPos;
//        auto lu = strnlen_sm( pCurr, length - currPos);
//        auto u =  std::string( pCurr, lu); //std::min(l, length));
//        m_users.push_back(u);
//        currPos += lu;
//    }
//}
//
//
//void CCompanyTask::encode(std::vector<char> & targ)
//{
//    auto l = strnlen_sm(data, length);
//    targ.push_back()
//    m_comp_name =  std::string(data, l); //std::min(l, length));
//    auto l2 = m_comp_name.length();
//    if (l2 < length)
//        m_usCount = data[l2];
//    auto currPos = l2 + sizeof(char);
//    for (auto i = 0; (i < m_usCount) && (currPos <= length); ++i)
//    {
//        auto pCurr = data + currPos;
//        auto lu = strnlen_sm( pCurr, length - currPos);
//        auto u =  std::string( pCurr, lu); //std::min(l, length));
//        m_users.push_back(u);
//        currPos += lu;
//    }
//}
//__cdecl
size_t  strnlen_sm(const char *str, size_t maxsize)
{
    size_t n;

    for (n = 0; n < maxsize && *str; n++, str++)
        ;

    return n;
}

//
//{
//    struct timespec ts;
//    if(clock_gettime(CLOCK_MONOTONIC,&ts) != 0) {
//     //error
//    }
//}

// boost::lexical_cast<std::string>
// #include <boost/lexical_cast.hpp>
std::string GetTickStr()
{
   //long int i=  (std::chrono::high_resolution_clock::now().rep);
    // using namespace date;
    using namespace std::chrono;

        //set time_point to current time
    //std::chrono::time_point<std::chrono::system_clock> time_point;
    auto time_point = std::chrono::high_resolution_clock::now();

    std::time_t ttp = std::chrono::high_resolution_clock::to_time_t(time_point);
    //std::cout << "time: " <<
    std::string str = std::ctime(&ttp);
    std::string  str2(str.c_str(), std::max(1ul, str.length()) - 1);
   // str += "_";
    return str2;
   // std::cout << system_clock::now() << " UTC\n";


}
