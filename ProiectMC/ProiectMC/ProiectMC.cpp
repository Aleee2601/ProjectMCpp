// ProiectMC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Player.h"

int main() {
 
    Player player1(1, "Alex", 0, 0);
 
    player1.setPosition(5, 10);
    player1.addScore(150);
    player1.displayStatus();

 
    int x, y;
    player1.getPosition(x, y);
    std::cout << "Pozitia curenta obtinuta: (" << x << ", " << y << ")" << std::endl;

   
    std::cout << "Scorul curent: " << player1.getScore() << std::endl;

    return 0;
}
