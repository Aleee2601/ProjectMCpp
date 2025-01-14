#include "../include/ClientFunctions.h"
#include <iostream>

int main() {
    ClientFunctions clientFunctions;
    int choice;
    int playerId;
    std::string direction;

    do {
        std::cout << "\n=== Client Menu ===\n";
        std::cout << "1. Move Player\n";
        std::cout << "2. Get Player Position\n";
        std::cout << "3. Get Player Score\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "Enter Player ID: ";
            std::cin >> playerId;
            std::cout << "Enter Direction (up, down, left, right): ";
            std::cin >> direction;
            clientFunctions.movePlayer(playerId, direction);
            break;

        case 2:
            std::cout << "Enter Player ID: ";
            std::cin >> playerId;
            clientFunctions.getPlayerPosition(playerId);
            break;

        case 3:
            std::cout << "Enter Player ID: ";
            std::cin >> playerId;
            clientFunctions.getPlayerScore(playerId);
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
