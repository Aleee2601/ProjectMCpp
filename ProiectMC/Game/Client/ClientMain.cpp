//#include <iostream>
//#include <string>
//#include "ClientLogic.h"
//
//int main(int argc, char* argv[])
//{
//    // Implicit, ne conectăm la un server local, port 8080.
//    // Dacă vrei, poți să iei adresa serverului din linia de comandă (argv).
//    std::string serverUrl = "http://localhost:8080/";
//
//    if (argc > 1)
//    {
//        // Permitem să specifici alt url/port: ex: ./client http://192.168.1.10:9999/
//        serverUrl = argv[1];
//    }
//
//    std::cout << "Se ruleaza Client Logic, conectat la: " << serverUrl << std::endl;
//
//    // Instanțiem clientul și apelăm funcția principală "run()"
//    ClientLogic client(serverUrl);
//    
//    client.run();
//
//    std::cout << "Client oprit. La revedere!\n";
//    return 0;
//}
#include "ClientLogic.h"

int main(int argc, char* argv[])
{
    // Dacă serverul rulează pe localhost:18080
    std::string serverUrl = "http://127.0.0.1:18080";

    // Creăm obiectul de logică client, care știe de serverUrl
    ClientLogic client(serverUrl);
    client.run();

    return 0;
}

