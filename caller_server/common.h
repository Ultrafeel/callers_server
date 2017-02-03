#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <cserverstatus.h>
#include "ccompanytask.h"
class CInteractor
{
public:
    virtual ~CInteractor() {}
    virtual void deliver(const CServerStatus& msg) = 0;
};

typedef boost::shared_ptr<CInteractor> CInteractor_ptr;


struct CTask_to_handle
{
    CCompanyTask m_task;
    CInteractor_ptr m_client;
};


#endif // COMMON_H_INCLUDED