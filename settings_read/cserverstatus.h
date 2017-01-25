#ifndef CSERVERSTATUS_H
#define CSERVERSTATUS_H

#include <string>
class CServerStatus
{
    public:
        CServerStatus();
        virtual ~CServerStatus();

        std::string message;

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & message;
    }
//        enum { message_length = 128};
//        char message[message_length]
    protected:

    private:
};

#endif // CSERVERSTATUS_H
