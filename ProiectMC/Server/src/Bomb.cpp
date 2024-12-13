#include "Bomb.h"
#include "map.h"
#include "Player.h"
#include <iostream>
#include <vector>

// Constructor: Initializes the bomb's position.
Bomb::Bomb(int x, int y) : m_x(x), m_y(y) {}

// Detonates the bomb and destroys walls in the surrounding cells.
void Bomb::Detonate(Map& map) {
    // Iterate through neighboring cells (4 cells around the bomb)
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i != 0 || j != 0) { // Skip the bomb's current cell
                map.DestroyWall(m_x + i, m_y + j); // Destroy walls around the bomb
            }
        }
    }
}

// Calculates the effects of the explosion on walls and players.
void Bomb::CalculateExplosionEffects(Map& map, std::vector<Player>& players) {
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int newX = m_x + i;
            int newY = m_y + j;

            // Check if the cell is within bounds and is a destructible wall
            if (map.IsWithinBounds(newX, newY)) {
                if (map.GetCellType(newX, newY) == CellType::DESTRUCTIBLE_WALL) {
                    std::cout << "Explosion destroys wall at (" << newX << ", " << newY << ")\n";
                }
            }

            // Check if any player is in the explosion radius
            for (auto& player : players) {
                int playerX, playerY;
                player.GetPosition(playerX, playerY);
                if (playerX == newX && playerY == newY) {
                    player.TakeHit(); // Apply damage to the player
                    std::cout << "Player " << player.GetName() << " is hit by explosion at ("
                        << newX << ", " << newY << ")\n";
                }
            }
        }
    }
}
