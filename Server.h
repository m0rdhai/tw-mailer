#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <atomic>
#include "MailSpooler.cpp"

class Server {
private:
    int port_;
    std::string spoolDir_;
    MailSpooler spooler_;
    int serverSocket_;
    std::atomic<bool> running_;

    bool readLine(int clientSocket, std::string& outLine);
    void sendResponse(int clientSocket, const std::string& response);
    void handleClient(int clientSocket);

public:
    Server(int port, const std::string& spoolDir);
    ~Server();
    void start();
};

#endif // SERVER_H