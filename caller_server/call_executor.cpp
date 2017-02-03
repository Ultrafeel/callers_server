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
        CInteractor_ptr  client = th.m_client;
        if (client.get())
            return;

        CCompanyTask  const& ct = th.m_task;
        cout << "task ";
        cout <<ct.m_comp_name << endl ;
        client->deliver(CServerStatus( "compon name " +  ct.m_comp_name));
        size_t iu = 1;
        for (CAbonent const& us : ct.m_abonents)
        {
            cout << ++iu << " abonent " << us.m_name << endl;

            client->deliver(CServerStatus(us.m_name));
        }
        client->deliver(CServerStatus( ct.m_comp_name + " ended" ));

    }
