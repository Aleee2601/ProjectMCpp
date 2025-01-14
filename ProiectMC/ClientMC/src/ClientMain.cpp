#include "../include/ClientFunctions.h"
#include <iostream>

int main() {
    ClientFunctions clientFunctions;
    int choice;

    do {
        std::cout << "\n=== Game Client Menu ===\n";
        std::cout << "1. Add Player\n";
        std::cout << "2. List Players\n";
        std::cout << "3. Update Player Position\n";
        std::cout << "4. View Map\n";
        std::cout << "5. Reset Game\n";
        std::cout << "0. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            clientFunctions.addPlayer();
            break;
        case 2:
            clientFunctions.listPlayers();
            break;
        case 3:
            clientFunctions.updatePlayerPosition();
            break;
        case 4:
            clientFunctions.viewMap();
            break;
        case 5:
            clientFunctions.resetGame();
            break;
        case 0:
            std::cout << "Exiting...\n";
            break;
        default:
            std::cerr << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
