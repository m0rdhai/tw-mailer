#include "Server.h"
#include "ProtocolSerializerValidator.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

Server::Server(int port, const std::string& spoolDir)
    : port_(port), spoolDir_(spoolDir), spooler_(spoolDir), serverSocket_(-1), running_(false) {}

Server::~Server() {
    if (serverSocket_ != -1) {
        close(serverSocket_);
    }
}

bool Server::readLine(int clientSocket, std::string& outLine) {
    outLine.clear();
    char ch;
    while (true) {
        ssize_t bytesRead = recv(clientSocket, &ch, 1, 0);
        if (bytesRead <= 0) {
            // Verbindung wurde getrennt
            return false;
        }
        if (ch == '\n') break;
        if (ch != '\r') {
            outLine += ch;
        }
    }
    return true;
}

void Server::sendResponse(int clientSocket, const std::string& response) {
    send(clientSocket, response.c_str(), response.length(), 0);
}

void Server::handleClient(int clientSocket) {
    while (true) {
        std::string command;
        if (!readLine(clientSocket, command) || command.empty()) {
            break;
        }

        if (command == "SEND") {
            std::string sender, receiver, subject;
            if (!readLine(clientSocket, sender) ||
                !readLine(clientSocket, receiver) ||
                !readLine(clientSocket, subject)) {
                break;
            }

            std::string message;
            bool success = true;
            while (true) {
                std::string line;
                if (!readLine(clientSocket, line)) {
                    success = false;
                    break;
                }
                if (line == ".") break;
                message += line + "\n";
            }

            if (success &&
                ProtocolSerializerValidator::validateUsername(sender) &&
                ProtocolSerializerValidator::validateUsername(receiver) &&
                ProtocolSerializerValidator::validateSubject(subject)) {
                if (spooler_.saveMessage(sender, receiver, subject, message)) {
                    sendResponse(clientSocket, "OK\n");
                } else {
                    sendResponse(clientSocket, "ERR\n");
                }
            } else {
                sendResponse(clientSocket, "ERR\n");
            }
        }
        else if (command == "LIST") {
            std::string username;
            if (!readLine(clientSocket, username)) break;

            if (ProtocolSerializerValidator::validateUsername(username)) {
                std::vector<std::string> subjects = spooler_.listSubjects(username);
                std::string res = std::to_string(subjects.size()) + "\n";
                for (const auto& subj : subjects) {
                    res += subj + "\n";
                }
                sendResponse(clientSocket, res);
            } else {
                sendResponse(clientSocket, "0\n");
            }
        }
        else if (command == "READ") {
            std::string username, msgNumStr;
            if (!readLine(clientSocket, username) ||
                !readLine(clientSocket, msgNumStr)) break;

            try {
                int msgNum = std::stoi(msgNumStr);
                std::string sender, receiver, subject, content;
                if (ProtocolSerializerValidator::validateUsername(username) &&
                    spooler_.readMessage(username, msgNum, sender, receiver, subject, content)) {
                    std::string res = "OK\n" + sender + "\n" + receiver + "\n" + subject + "\n" + content + ".\n";
                    sendResponse(clientSocket, res);
                } else {
                    sendResponse(clientSocket, "ERR\n");
                }
            } catch (...) {
                sendResponse(clientSocket, "ERR\n");
            }
        }
        else if (command == "DEL") {
            std::string username, msgNumStr;
            if (!readLine(clientSocket, username) ||
                !readLine(clientSocket, msgNumStr)) break;

            try {
                int msgNum = std::stoi(msgNumStr);
                if (ProtocolSerializerValidator::validateUsername(username) &&
                    spooler_.deleteMessage(username, msgNum)) {
                    sendResponse(clientSocket, "OK\n");
                } else {
                    sendResponse(clientSocket, "ERR\n");
                }
            } catch (...) {
                sendResponse(clientSocket, "ERR\n");
            }
        }
        else if (command == "QUIT") {
            break;
        }
        else {
            sendResponse(clientSocket, "ERR\n");
        }
    }
    close(clientSocket);
}

void Server::start() {
    serverSocket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket_ < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    int opt = 1;
    setsockopt(serverSocket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port_);

    if (bind(serverSocket_, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return;
    }

    if (listen(serverSocket_, 5) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return;
    }

    std::cout << "Server listening on port " << port_ << " with spool directory: " << spoolDir_ << std::endl;
    running_ = true;

    while (running_) {
        sockaddr_in clientAddr{};
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket_, (struct sockaddr*)&clientAddr, &clientLen);
        if (clientSocket < 0) {
            if (!running_) break;
            continue;
        }
        handleClient(clientSocket);
    }
}