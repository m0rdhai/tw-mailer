#include <iostream>
#include <string>
#include "ProtocolSerializerValidator.h"
#include "ClientSocket.h"

int main(int argc, char* argv[]) 
{

    std::string ip = "127.0.0.1";
    int port = 6543; // Standard-Port aus der moodle ServerSample

    if (argc >= 2) 
    {
        ip = argv[1];
    }
    if (argc >= 3) 
    {
        port = std::stoi(argv[2]);
    }

    ClientSocket client;
    if (!client.connectToServer(ip, port))
    {
        return 1;
    }

    std::string welcomeMessage = client.readline();
    std::cout << "<< " << welcomeMessage;

    ProtocolSerializerValidator validator;
    std::string userInput;
    do {
        std::cout << ">> ";
        if (!std::getline(std::cin, userInput)) 
        {
            break;
        }

        std::string payload;
        if (userInput == "quit") 
        {
            payload = validator.serializeQuit();
        }
         else
        {
            payload = userInput + "\n";
        }

        if (!client.sendData(payload)) 
        {
            break;
        }

        std::string response = client.readline();
        if (response.empty()) 
        {
            break;
        }
        std::cout << "<< " << response << "\n";

    } while (userInput != "quit");

    std::cout << "Programm beendet.\n";
    return 0;
}



    /** 
    ProtocolSerializerValidator validator;

    std::cout << "Testfaelle nur. Danach löschen, wenn wir fertig getestet haben \n\n";

    // 1. Test: Username Validierung
    std::cout << "1. Test: Username\n";
    std::cout << "Test 'alice' (sollte 1 / true sein): " << validator.validateUsername("alice") << "\n";
    std::cout << "Test 'Alice!' (sollte 0 / false sein, wegen Großbuchstabe & Sonderzeichen): " << validator.validateUsername("Alice!") << "\n";
    std::cout << "Test 'zuuuuuulang' (sollte 0 / false sein, über 8 Zeichen): " << validator.validateUsername("zuuuuuulang") << "\n\n";

    // 2. Test: Subject Validierung
    std::cout << "2. Test: Subject\n";
    std::cout << "Test 'Wichtige Mail' (sollte 1 / true sein): " << validator.validateSubject("Wichtige Mail") << "\n";
    std::cout << "Test '' (sollte 0 / false sein, leer): " << validator.validateSubject("") << "\n\n";

    // 3. SEND
    std::string pSend = validator.serializeSend("alice", "bob", "Meeting", "Hallo Bob,\nwann treffen wir uns?");
    std::cout << "Paket: " << pSend << "--------------------\n";

    // 4. LIST
    std::string pList = validator.serializeList("bob");
    std::cout << "Paket: " << pList << "--------------------\n";

    // 5. READ
    std::string pRead = validator.serializeRead("bob", "1");
    std::cout << "Paket: " << pRead << "--------------------\n";

    // 6. DEL
    std::string pDel = validator.serializeDel("bob", "1");
    std::cout << "Paket: " << pDel << "--------------------\n";

    // 7. QUIT
    std::string pQuit = validator.serializeQuit();
    std::cout << "Paket: " << pQuit << "--------------------\n";

    return 0;
    **/
   