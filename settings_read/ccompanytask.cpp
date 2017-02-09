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
