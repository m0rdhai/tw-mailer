#include "ClientSocket.h"
#include <iostream>
#include <cstring>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

ClientSocket::ClientSocket() 
{
    m_socket = -1;
}

ClientSocket::~ClientSocket() 
{
    closeConnection();
}

bool ClientSocket::connectToServer(std::string ip, int port) 
{
    //Socket erstellen 
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == -1) 
    {
        std::perror("Socket error");
        return false;
    }

    //Adresse initialisieren
    struct sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &address.sin_addr) <= 0)
    {
        std::cerr << "Ungültige IP-Adresse.\n";
        close(m_socket);
        m_socket = -1;
        return false;
    }

    //Verbindung aufbauen
    std::cout << "Verbinde mit Server " << ip << ":" << port << "...\n";
    if (connect(m_socket, (struct sockaddr*)&address, sizeof(address)) == -1) 
    {
        std::perror("Connect error - no server available");
        close(m_socket);
        m_socket = -1;
        return false;
    }

    std::cout << "Connection with server (" << ip << ") established\n";
    return true;
}

bool ClientSocket::sendData(std::string data) {
    if (m_socket == -1) return false;
    
    if (send(m_socket, data.c_str(), data.length(), 0) == -1) 
    {
        std::perror("send error");
        return false;
    }
    return true;
}

std::string ClientSocket::readline() 
{
    if (m_socket == -1) 
    {
        std::cerr << "Konnte nicht lesen: Kein Socket verbunden\n";
        return "";
    }
    std::string result = "";
    char currentBuffer;
    
    while (true) 
    {
        int bytesRead = recv(m_socket, &currentBuffer, 1, 0);
        if (bytesRead == 1) 
        {
            result += currentBuffer;
            if (currentBuffer == '\n') 
            {
                break;
            }
        } 
        else 
        {
            break; 
        }
    }
    return result;
}

void ClientSocket::closeConnection() 
{
    if (m_socket != -1) 
    {
        shutdown(m_socket, SHUT_RDWR);
        close(m_socket);
        m_socket = -1;
    }
}