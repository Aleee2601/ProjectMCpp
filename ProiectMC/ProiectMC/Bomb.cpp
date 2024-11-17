#include "Bomb.h"
#include "Map.h"
#include "Player.h"
#include <iostream>
#include <vector>

Bomb::Bomb(int x, int y) : x(x), y(y) {}

void Bomb::detonate(Map& map) {
    // Iteram prin vecini (4 celule din jur)
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i != 0 || j != 0) {  // Evitam celula centrala
                map.destroyWall(x + i, y + j);
            }
        }
    }
}

void Bomb::calculateExplosionEffects(Map& map, std::vector<Player>& players) {
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int newX = x + i;
            int newY = y + j;

            if (map.isWithinBounds(newX, newY)) {
                if (map.getCellType(newX, newY) == CellType::DESTRUCTIBLE_WALL) {
                    std::cout << "Explosion destroys wall at (" << newX << ", " << newY << ")\n";
                }
            }

            for (auto& player : players) {
                int playerX, playerY;
                player.getPosition(playerX, playerY);
                if (playerX == newX && playerY == newY) {
                    player.takeHit();
                    std::cout << "Player " << player.getName() << " is hit by explosion at (" << newX << ", " << newY << ")\n";

                }
            }
        }
    }
}