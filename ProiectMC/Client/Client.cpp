#include "ClientFunctions.h"   // Nu este nevoie de ../include, deoarece include este adăugat în proiect
#include <iostream>

int main() {
    int choice = 0;

    do {
        std::cout << "\nMenu:\n";
        std::cout << "1. List Players\n";
        std::cout << "2. Add Player\n";
        std::cout << "3. Update Player Position\n";
        std::cout << "4. View Map\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            ClientFunctions::listPlayers();
            break;
        case 2:
            ClientFunctions::addPlayer();
            break;
        case 3:
            ClientFunctions::updatePlayerPosition();
            break;
        case 4:
            ClientFunctions::viewMap();
            break;
        case 0:
            std::cout << "Exiting...\n";
            break;
        default:
            std::cerr << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}
