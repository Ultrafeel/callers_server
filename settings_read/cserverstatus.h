#ifndef CSERVERSTATUS_H
#define CSERVERSTATUS_H

#include <string>
class CServerStatus
{
    public:
    enum TEnum
    {
        ENormal,
        ELastMark,
        EServerErrorReadCompanies
    } ;
        CServerStatus();
        CServerStatus(std::string const& str): message(str), m_enum(ENormal){};
        CServerStatus( TEnum en): message(getMess(en)), m_enum(en)
         {};
        CServerStatus(std::string const& str, TEnum en): message(str), m_enum(en)
         {};
       virtual ~CServerStatus();

        std::string message;

    bool isLastMark() const
    {
        return (getLastMark() == message);
    }

   TEnum  m_enum;

    static  char const * getMess(TEnum en)
    {
        switch (en)
        {
           case EServerErrorReadCompanies:
                return "\n Server terminated due to error while reading companies.";
           case ELastMark:
                return getLastMark();
           default:
                return "";
        }
    }
    bool IsServerErrorReadCompanies() const
    {
        return (EServerErrorReadCompanies == m_enum);
    }

    template <typename Archive>
    void serialize(Archive& ar, int version)
    {
        ar & message;
        int32_t en = m_enum;
        ar & en;//const_cast<TEnum>
        //ar & m_enum;
        m_enum = static_cast<TEnum>(en);
    }
//        enum { message_length = 128};
//        char message[message_length]
    static  char const * getLastMark()
    {
        return "\n isLastMark";
    }
      private:


};

#endif // CSERVERSTATUS_H
