#include "call_executor.h"
#include <iostream>
call_executor::call_executor( executor_pool_base & pool): m_pool(pool)
{
    //ctor
}

call_executor::~call_executor()
{
    //dtor
}

// static
char  const * caller_executor_pool::endMessage = "\n Have not any tasks";

using namespace std;
void call_executor::CallCompanyTask(CTask_to_handle const& th)
{
    CInteractor_ptr  client = th.m_client;
    if (!client.get())
        return;
    if (!th.m_task.get())
    {
        assert(0);
        return;
    }
    CCompanyTask  const& ct = *th.m_task;
    cout << "task :";
    cout << ct.m_comp_name << ". priority : " << ct.m_priority << "\n user count: " << ct.m_abonents.size() << endl ;
    m_pool.deliver_to_client(client,CServerStatus( "Company name :" +  ct.m_comp_name));
    size_t iu = 0;
    for (CAbonent const& us : ct.m_abonents)
    {
        cout << ++iu << ". Calling abonent " << us.m_name << " num:" << us.m_number<< endl;
        //emulate call
        cout << "advertise : " << ct.m_advertise<< "." << endl;
        sleep(1);

        m_pool.deliver_to_client(client,CServerStatus("  call to abonent : " + us.m_name + " finished"));
    }
    m_pool.deliver_to_client(client,CServerStatus( ct.m_comp_name + " ended." ));
    m_pool.CheckIfLeft(client);

}






//The code I am using, based on reading /proc

#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

//Then, from inside a funcion:
int procTC()
{
    DIR *proc_dir;
    {
        char dirname[100];
        snprintf(dirname, sizeof dirname, "/proc/%d/task", getpid());
        proc_dir = opendir(dirname);
    }

    if (proc_dir)
    {
        /* /proc available, iterate through tasks... */

        int tc =  0;


        struct dirent *entry;
        while ((entry = readdir(proc_dir)) != NULL)
        {
            if(entry->d_name[0] == '.')
                continue;

            //int tid = atoi(entry->d_name);
            tc++;
            /* ... (do stuff with tid) ... */
        }

        closedir(proc_dir);
        return tc;

    }
    else
    {
        int tc =  -1;
        return tc;
        /* /proc not available, act accordingly */
    }

}
