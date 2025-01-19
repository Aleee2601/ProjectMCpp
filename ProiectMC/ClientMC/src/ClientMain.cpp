//#include "../include/ClientFunctions.h" // Asigură-te că această cale este corectă
//#include <iostream>
//#include <vector>
//#include <conio.h> // Pentru a capta tastele fără ENTER pe Windows
//#include <cstdlib>
//
//const int MAP_WIDTH = 10;
//const int MAP_HEIGHT = 10;
//
//// Funcția pentru afișarea hărții
//void displayMap(const std::vector<std::vector<char>>& map) {
//    system("cls"); // Curăță ecranul (folosește "clear" pe Linux)
//    for (const auto& row : map) {
//        for (const auto& cell : row) {
//            std::cout << cell << ' ';
//        }
//        std::cout << '\n';
//    }
//}
//
//int main() {
//    // Inițializarea hărții cu spații goale
//    std::vector<std::vector<char>> map(MAP_HEIGHT, std::vector<char>(MAP_WIDTH, '.'));
//
//    ClientFunctions client;
//    int playerId;
//    std::string name;
//
//    std::cout << "Enter your Player ID: ";
//    std::cin >> playerId;
//    std::cout << "Enter your name: ";
//    std::cin >> name;
//
//    // Înregistrăm jucătorul pe server
//    client.registerPlayer(playerId, name);
//
//    // Setăm poziția inițială a jucătorului (de ex., centru)
//    int playerX = MAP_WIDTH / 2;
//    int playerY = MAP_HEIGHT / 2;
//    map[playerY][playerX] = 'X'; // Adăugăm jucătorul pe hartă
//    displayMap(map);
//
//    char command;
//    std::cout << "Use W/A/S/D to move the player (X). Press Q to quit.\n";
//
//    while (true) {
//        command = _getch(); // Citește comanda utilizatorului
//
//        // Eliminăm jucătorul de pe poziția curentă
//        map[playerY][playerX] = '.';
//
//        // Procesăm inputul utilizatorului
//        if (command == 'w' || command == 'W') {
//            if (playerY > 0) playerY--;
//        }
//        else if (command == 's' || command == 'S') {
//            if (playerY < MAP_HEIGHT - 1) playerY++;
//        }
//        else if (command == 'a' || command == 'A') {
//            if (playerX > 0) playerX--;
//        }
//        else if (command == 'd' || command == 'D') {
//            if (playerX < MAP_WIDTH - 1) playerX++;
//        }
//        else if (command == 'q' || command == 'Q') {
//            break; // Ieșim din joc
//        }
//
//        // Trimitem mutarea către server
//        std::string direction;
//        if (command == 'w' || command == 'W') direction = "up";
//        else if (command == 'a' || command == 'A') direction = "left";
//        else if (command == 's' || command == 'S') direction = "down";
//        else if (command == 'd' || command == 'D') direction = "right";
//        client.movePlayer(playerId, direction);
//
//        // Actualizăm poziția jucătorului
//        map[playerY][playerX] = 'X';
//
//        // Afișăm harta
//        displayMap(map);
//    }
//
//    std::cout << "Game Over. Thank you for playing!\n";
//    return 0;
//}
#include "../include/ClientLogic.h"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
       //  Instanțiază logica clientului
        ClientLogic client;

        // Rulează clientul
        client.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Eroare în aplicație: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

