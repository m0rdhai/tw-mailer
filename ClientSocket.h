#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include <string>
#include <sys/types.h>

class ClientSocket {

public:
    ClientSocket();
    ~ClientSocket();
    void closeConnection();
    bool connectToServer(std::string ip, int port);
    bool sendData(std::string data);
    std::string readline();
  

private:
    int m_socket;
};

#endif