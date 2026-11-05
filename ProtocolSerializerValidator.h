#ifndef PROTOCOL_SERIALIZER_VALIDATOR_H
#define PROTOCOL_SERIALIZER_VALIDATOR_H

#include <string>

class ProtocolSerializerValidator {
public:
    bool validateUsername(std::string username);
    bool validateSubject(std::string subject);

    std::string serializeQuit();
    std::string serializeList(std::string username);
    std::string serializeRead(std::string username,std::string messageNumber);
    std::string serializeDel(std::string username,std::string messageNumber);
    std::string serializeSend(std::string sender,std::string receiver,std::string subject,std::string message);
    
};

#endif 