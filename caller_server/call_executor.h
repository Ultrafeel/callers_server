#ifndef CALL_EXECUTOR_H
#define CALL_EXECUTOR_H

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "common.h"
#include <boost/asio.hpp>
class call_executor
    : //public CInteractor,
      public boost::enable_shared_from_this<call_executor>
{
     public:
        call_executor();
        virtual ~call_executor();

        boost::asio::io_service *  m_pio_service;
    protected:

    private:
};


#endif // CALL_EXECUTOR_H
