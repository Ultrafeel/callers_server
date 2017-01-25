#ifndef CCOMPANYTASK_H
#define CCOMPANYTASK_H

#include <string>
#include <deque>
#include <vector>

//__cdecl
size_t  strnlen_sm(const char *str, size_t maxsize);

class CCompanyTask
{
public:
    CCompanyTask();
    virtual ~CCompanyTask();


    std::string m_comp_name;
    uint16_t            m_priority;
    std::string  m_advertise;

    std::deque<std::string> m_abonents;

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

#endif // CCOMPANYTASK_H
