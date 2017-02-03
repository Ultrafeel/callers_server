#ifndef CSERVERSTATUS_H
#define CSERVERSTATUS_H

#include <string>
class CServerStatus
{
    public:
        CServerStatus();
        CServerStatus(std::string const& str): message(str){};
        virtual ~CServerStatus();

        std::string message;
   static  char const * getLastMark()
    {
        return "\n isLastMark";
    }

    bool isLastMark() const
    {
        return (getLastMark() == message);
    }
    template <typename Archive>
    void serialize(Archive& ar, int version)
    {
        ar & message;
    }
//        enum { message_length = 128};
//        char message[message_length]
    protected:

    private:
};

#endif // CSERVERSTATUS_H
