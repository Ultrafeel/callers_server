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
    std::deque<std::string> m_users;

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & m_comp_name;
        ar & m_users;
    }
//    unsigned char m_usCount;
//    void decode(char const * data, size_t length);
//  void encode(std::vector<char> & targ);
protected:

private:
};

#endif // CCOMPANYTASK_H
