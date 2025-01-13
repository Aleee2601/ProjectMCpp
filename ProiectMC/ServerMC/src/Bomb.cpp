#include "../include/Bomb.h"
#include <iostream>
#include <vector>

Bomb::Bomb(int x, int y) : m_x(x), m_y(y) {}

// Detonates the bomb using a callback to destroy walls
void Bomb::Detonate(std::function<void(int, int)> destroyWallCallback) {
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i != 0 || j != 0) {
                destroyWallCallback(m_x + i, m_y + j); // Apelăm callback-ul
            }
        }
    }
}
// Calculates the effects of the explosion on walls and players.
void Bomb::CalculateExplosionEffects(
    std::function<bool(int, int)> isWallDestructibleCallback,
    std::function<void(int, int)> destroyWallCallback,
    std::function<void(int, int)> applyDamageToPlayerCallback
) {
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int newX = m_x + i;
            int newY = m_y + j;

            // Check and destroy walls
            if (isWallDestructibleCallback(newX, newY)) {
                destroyWallCallback(newX, newY);
                std::cout << "Explosion destroys wall at (" << newX << ", " << newY << ")\n";
            }

            // Apply damage to players
            applyDamageToPlayerCallback(newX, newY);
        }
    }
}
bool Bomb::IsInactive() {
    // Exemplu logică: schimbă cu logica ta reală
    return false;
}