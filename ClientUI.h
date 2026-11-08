#ifndef CLIENT_UI_H
#define CLIENT_UI_H

#include "ClientSocket.h"
#include "ProtocolSerializerValidator.h"

class ClientUI {
public:
    void run(ClientSocket& client);
    void printMenu();

private:
    ProtocolSerializerValidator m_validator;
};

#endif