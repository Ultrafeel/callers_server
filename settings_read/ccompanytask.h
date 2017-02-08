#ifndef CCOMPANYTASK_H
#define CCOMPANYTASK_H

#include <string>
#include <deque>
#include <vector>
//for shared_ptr
#include <memory>
#include <stdint.h>

//__cdecl
size_t  strnlen_sm(const char *str, size_t maxsize);

class CAbonent final
{
public:
    std::string m_name;
    std::string m_number;
    template <typename Archive>
    void serialize(Archive& ar, int version)
    {
        ar & m_name ;
        ar & m_number;
    }
};

struct TCompanyTask
{
    std::string  m_comp_name;
    uint16_t     m_priority;
    std::string  m_advertise;

    std::deque<CAbonent> m_abonents;
};
class CCompanyTask : public TCompanyTask
{
public:
    static char const * getTerminatedName()
    {
        return "\n No more Companies";
    }
    bool IsTerminatorCompany() const
    {
        return (m_comp_name == getTerminatedName());
    }

    CCompanyTask();
    CCompanyTask(TCompanyTask const & ct):TCompanyTask(ct) {};
    virtual ~CCompanyTask();

//
//    std::string m_comp_name;
//    uint16_t            m_priority;
//    std::string  m_advertise;
//
//    std::deque<std::string> m_abonents;
    //  enum {  max_message_length = 111 };
    template <typename Archive>
    void serialize(Archive& ar, int version)
    {
        ar & m_comp_name;
        ar & m_priority;
        ar & m_advertise;
        ar & m_abonents;
    }
//    unsigned char m_usCount;
//    void decode(char const * data, size_t length);
//  void encode(std::vector<char> & targ);
protected:

private:
};
typedef  std::deque<CCompanyTask> TInitiaWriteData;
typedef  std::shared_ptr<CCompanyTask> CCompanyTask_ptr;

#endif // CCOMPANYTASK_H
