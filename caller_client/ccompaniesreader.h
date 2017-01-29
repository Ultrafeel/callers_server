#ifndef CCOMPANIESREADER_H
#define CCOMPANIESREADER_H
#include <deque>
#include "CSettingsReader.h"
#include "ccompanytask.h"

class CCompaniesReader : public CSettingsReader
{
    public:
        CCompaniesReader(char const* path);
        virtual ~CCompaniesReader();
        bool OpenAndReadTasks();

        std::deque<CCompanyTask> m_tasks;
    protected:

    private:
};

#endif // CCOMPANIESREADER_H
