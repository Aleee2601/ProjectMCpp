//#include "../include/ClientFunctions.h"
//#include <iostream>
//
//int main() {
//    ClientFunctions clientFunctions;
//    int choice;
//    int playerId;
//    std::string direction;
//
//    do {
//        std::cout << "\n=== Client Menu ===\n";
//        std::cout << "1. Move Player\n";
//        std::cout << "2. Get Player Position\n";
//        std::cout << "3. Get Player Score\n";
//        std::cout << "0. Exit\n";
//        std::cout << "Enter your choice: ";
//        std::cin >> choice;
//
//        switch (choice) {
//        case 1:
//            std::cout << "Enter Player ID: ";
//            std::cin >> playerId;
//            std::cout << "Enter Direction (up, down, left, right): ";
//            std::cin >> direction;
//            clientFunctions.movePlayer(playerId, direction);
//            break;
//
//        case 2:
//            std::cout << "Enter Player ID: ";
//            std::cin >> playerId;
//            clientFunctions.getPlayerPosition(playerId);
//            break;
//
//        case 3:
//            std::cout << "Enter Player ID: ";
//            std::cin >> playerId;
//            clientFunctions.getPlayerScore(playerId);
//            break;
//
//        case 0:
//            std::cout << "Exiting...\n";
//            break;
//
//        default:
//            std::cerr << "Invalid choice. Try again.\n";
//        }
//    } while (choice != 0);
//
//    return 0;
//}
#include <iostream>
#include <vector>
#include <conio.h> // Pentru a capta tastele fără ENTER pe Windows
#include <cstdlib>

// Dimensiunile hărții
const int MAP_WIDTH = 10;
const int MAP_HEIGHT = 10;

// Funcția pentru afișarea hărții
void displayMap(const std::vector<std::vector<char>>& map) {
    system("cls"); // Curăță ecranul (folosește "clear" pe Linux)
    for (const auto& row : map) {
        for (const auto& cell : row) {
            std::cout << cell << ' ';
        }
        std::cout << '\n';
    }
}

int main() {
    // Inițializarea hărții cu spații goale (.)
    std::vector<std::vector<char>> map(MAP_HEIGHT, std::vector<char>(MAP_WIDTH, '.'));

    // Poziția inițială a jucătorului
    int playerX = MAP_WIDTH / 2;
    int playerY = MAP_HEIGHT / 2;
    map[playerY][playerX] = 'X'; // Reprezentăm jucătorul cu 'X'

    // Afișăm harta inițială
    displayMap(map);

    char input;
    std::cout << "Use W/A/S/D to move the player (X). Press Q to quit.\n";

    while (true) {
        input = _getch(); // Citește o tastă de la utilizator fără așteptarea ENTER-ului

        // Eliminăm jucătorul de pe poziția curentă
        map[playerY][playerX] = '.';

        // Procesăm inputul utilizatorului
        if (input == 'w' || input == 'W') {
            if (playerY > 0) playerY--; // Mergem în sus
        }
        else if (input == 's' || input == 'S') {
            if (playerY < MAP_HEIGHT - 1) playerY++; // Mergem în jos
        }
        else if (input == 'a' || input == 'A') {
            if (playerX > 0) playerX--; // Mergem la stânga
        }
        else if (input == 'd' || input == 'D') {
            if (playerX < MAP_WIDTH - 1) playerX++; // Mergem la dreapta
        }
        else if (input == 'q' || input == 'Q') {
            break; // Ieșim din joc
        }

        // Actualizăm poziția jucătorului
        map[playerY][playerX] = 'X';

        // Afișăm harta după mutare
        displayMap(map);
    }

    std::cout << "Game Over. Thank you for playing!\n";
    return 0;
}
