#include "ClientUI.h"
#include <iostream>
#include <string>

void ClientUI::printMenu() 
{
    std::cout << "\n(1) SEND | (2) LIST | (3) READ | (4) DEL | (5) QUIT\n";
    std::cout << "Choose an option (1-5): ";
}
void ClientUI::run(ClientSocket& client) 
{
    int choice = 0;

    do {
        printMenu();
        if (!(std::cin >> choice)) 
        {
            break;
        }
        std::cin.ignore();

        std::string payload = "";

        switch (choice) 
        {
            case 1: { 
                std::string sender, receiver, subject, message = "", line;
                
                std::cout << "Absender (max. 8 Zeichen): ";
                std::getline(std::cin, sender);
                
                std::cout << "Empfänger (max. 8 Zeichen): ";
                std::getline(std::cin, receiver);
                
                std::cout << "Betreff (max. 80 Zeichen): ";
                std::getline(std::cin, subject);
                
                std::cout << "Nachrichtentext (Beenden mit einem einzelnen '.' in einer Zeile):\n";
                while (true) 
                {
                    std::getline(std::cin, line);
                    if (!line.empty() && line.back() == '\r') 
                    {
                        line.pop_back();
                    }
                    if (line == ".") 
                    {
                        break;
                    }
                    message += line + "\n";
                }

                payload = m_validator.serializeSend(sender, receiver, subject, message);
                break;
            }
            case 2: 
            {
                std::string user;
                std::cout << "Benutzername vom Empfaenger: ";
                std::getline(std::cin, user);
                payload = m_validator.serializeList(user);
                break;
            }
            case 3: 
            { 
                std::string user, msgNum;
                std::cout << "Benutzername vom Empfaenger: ";
                std::getline(std::cin, user);
                std::cout << "Nachrichtennummer: ";
                std::getline(std::cin, msgNum);
                payload = m_validator.serializeRead(user, msgNum);
                break;
            }
            case 4: 
            { 
                std::string user, msgNum;
                std::cout << "Benutzername vom Empfaenger: ";
                std::getline(std::cin, user);
                std::cout << "Nachrichtennummer zum Löschen: ";
                std::getline(std::cin, msgNum);
                payload = m_validator.serializeDel(user, msgNum);
                break;
            }
            case 5: 
            { 
                payload = m_validator.serializeQuit();
                break;
            }
            default:
                std::cout << "Ungültige Auswahl. Bitte wähle 1-5.\n";
                continue;
        }

       if (!client.sendData(payload)) 
        {
            std::cout << "Konnte Daten nicht senden.\n";
            break;
        }

        if (choice == 2) 
        {
            std::string countStr = client.readline();
            std::cout << "<< " << countStr << "\n";
            
            int count = 0;
            try {
                count = std::stoi(countStr);
            } catch (...) {
                count = 0;
            }

            for (int i = 0; i < count; ++i) 
            {
                std::string subject = client.readline();
                std::cout << "<< " << subject << "\n";
            }
        }
        else if (choice == 3) 
        {
            std::string response = client.readline();
            std::cout << "<< " << response << "\n";
    
            if (response.rfind("OK", 0) == 0) 
            {
                while (true) 
                {
                    std::string line = client.readline();
                    if (line.empty() || line == "." || line == ".\n") 
                    {
                        break;
                    }
                    std::cout << "<< " << line << "\n";
                }
            }
        }
        else 
        {
            std::string response;
            while (true) 
            {
                response = client.readline();
                if (response.empty() || response == ".\n" || response == ".") 
                {
                    break;
                }
                std::cout << "<< " << response << "\n";
                if (response.rfind("OK", 0) == 0 || response.rfind("ERR", 0) == 0) 
                {
                    break;
                }
            }
        }

    } 
    while (choice != 5);
}