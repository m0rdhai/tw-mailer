#include "ProtocolSerializerValidator.h"
#include <algorithm>
#include <cctype>

bool ProtocolSerializerValidator::validateUsername(std::string username) 
{
    if (username.empty() || username.length() > 8) 
    {
        return false;
    }
    for (char character : username) 
    {
        if (!((character >= '0' && character <= '9') || (character >= 'a' && character <= 'z'))) 
        {
            return false;
        }
    }
    return true;
}

bool ProtocolSerializerValidator::validateSubject(std::string subject) 
{
    if (subject.empty() || subject.length() > 80) 
    {
        return false;
    }
    return true;
}

std::string ProtocolSerializerValidator::serializeQuit() 
{
    return "QUIT\n";
}

std::string ProtocolSerializerValidator::serializeList(std::string username) 
{
    return "LIST\n" + username + "\n";
}

std::string ProtocolSerializerValidator::serializeRead(std::string username, std::string messageNumber) 
{
    return "READ\n" + username + "\n" + messageNumber + "\n";
}

std::string ProtocolSerializerValidator::serializeDel(std::string username, std::string messageNumber)
{
    return "DEL\n" + username + "\n" + messageNumber + "\n";
}

std::string ProtocolSerializerValidator::serializeSend(std::string sender, std::string receiver, std::string subject, std::string message) 
{
    return "SEND\n" + sender + "\n" + receiver + "\n" + subject + "\n" + message + ".\n";
}