#include "call_executor.h"
#include <iostream>
call_executor::call_executor()
{
    //ctor
}

call_executor::~call_executor()
{
    //dtor
}

 // static
  char  const * caller_executor_pool::endMessage = "\n end";

using namespace std;
void call_executor::CallCompanyTask(CTask_to_handle const& th)
{
        CInteractor_ptr  CInteractor = th.m_client;
        if (CInteractor.get())
            return;

        CCompanyTask  const& ct = th.m_task;
        cout << "task ";
        cout <<ct.m_comp_name << endl ;
        CInteractor->deliver(CServerStatus( "compon name " +  ct.m_comp_name));
        size_t iu = 1;
        for (CAbonent const& us : ct.m_abonents)
        {
            cout << ++iu << " abonent " << us.m_name << endl;

            CInteractor->deliver(CServerStatus(us.m_name ));
        }
        bool tasks_left = false;
        CInteractor->deliver(CServerStatus(endMessage));
        for (CTask_to_handle const& thi : read_queue)
        {
            if (thi.m_client.get() == CInteractor.get())
                tasks_left= true;
        }
        if (tasks_left)
            CInteractor->deliver(CServerStatus("All tasks end"));
        else
            CInteractor->deliver(CServerStatus("Have some your tasks"));

    }
