#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <cserverstatus.h>

class CInteractor
{
public:
    virtual ~CInteractor() {}
    virtual void deliver(const CServerStatus& msg) = 0;
};

typedef boost::shared_ptr<CInteractor> CInteractor_ptr;


#endif // COMMON_H_INCLUDED
