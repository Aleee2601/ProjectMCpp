// ProiectMC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
// import player;
#include "map.h"

int main() {
 
    /*
    Player player1(1, "Alex", 0, 0);
 
    player1.setPosition(5, 10);
    player1.addScore(150);
    player1.displayStatus();

 
    int x, y;
    player1.getPosition(x, y);
    std::cout << "Pozitia curenta obtinuta: (" << x << ", " << y << ")" << std::endl;

   
    std::cout << "Scorul curent: " << player1.getScore() << std::endl;
    */

    // Test 1: Generarea hărții
    Map map(10, 10);  
    map.displayMap(); 

    // Contorizare pentru fiecare tip de celulă
    int emptyCount = 0, destructibleCount = 0, indestructibleCount = 0;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            CellType type = map.getCellType(i, j);
            if (type == CellType::EMPTY) emptyCount++;
            else if (type == CellType::DESTRUCTIBLE_WALL) destructibleCount++;
            else if (type == CellType::INDESTRUCTIBLE_WALL) indestructibleCount++;
        }
    }

    // Print counts to verify diversity in generated map
    std::cout << "Empty cells: " << emptyCount << "\n";
    std::cout << "Destructible walls: " << destructibleCount << "\n";
    std::cout << "Indestructible walls: " << indestructibleCount << "\n";

    // Test 2: Coliziunea cu zidurile
    int testX = 5, testY = 5;
    map.setCellType(testX, testY, CellType::DESTRUCTIBLE_WALL);  
    if (map.isCollisionWithWall(testX, testY)) {
        std::cout << "Collision detected with destructible wall at (" << testX << ", " << testY << ")\n";
    }
    else {
        std::cout << "No collision detected at (" << testX << ", " << testY << ")\n";
    }

    // Test 3: Distrugerea zidurilor
    std::cout << "Attempting to destroy wall at (" << testX << ", " << testY << "):\n";
    map.destroyWallWithDisplay(testX, testY);

    return 0;
}
