#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <cserverstatus.h>
#include "ccompanytask.h"
class CInteractor:
    public boost::enable_shared_from_this<CInteractor>
{
public:
    virtual ~CInteractor() {}
private:
    virtual void deliver(const CServerStatus& msg) = 0;
    friend class caller_executor_pool;
};

typedef boost::shared_ptr<CInteractor> CInteractor_ptr;


struct CTask_to_handle
{
    CCompanyTask_ptr m_task;
    CInteractor_ptr m_client;
};

std::string GetTickStr();

#endif // COMMON_H_INCLUDED
