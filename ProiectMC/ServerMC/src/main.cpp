//#include <iostream>
//
//int main() {
//    std::cout << "Battle City Game Starting..." << std::endl;
//    return 0;
//}
#include "../include/Server.h"

int main() {
    Server server(8080);
    server.run();
    return 0;
}
