#include "../include/Bullet.h"
#include "../include/map.h"
#include "../include/Player.h"


#include <iostream>
#include <unordered_map>

// Constructor for a bullet with a starting position and direction
Bullet::Bullet(int startX, int startY, Direction dir)
    : m_x(startX), m_y(startY), m_direction(dir), m_endX(0), m_endY(0) {
}

// Constructor for a bullet with a starting and ending position, plus direction
Bullet::Bullet(int startX, int startY, int endX, int endY, Direction dir)
    : m_x(startX), m_y(startY), m_endX(endX), m_endY(endY), m_direction(dir) {
}

// Moves the bullet one step in its direction
void Bullet::Move() {
    static const std::unordered_map<Direction, std::pair<int, int>> directionOffsets = {
        {Direction::UP, {0, -1}},
        {Direction::DOWN, {0, 1}},
        {Direction::LEFT, {-1, 0}},
        {Direction::RIGHT, {1, 0}}
    };
    m_x += directionOffsets.at(m_direction).first;
    m_y += directionOffsets.at(m_direction).second;
}

// Checks if the bullet is out of the map bounds
bool Bullet::IsOutOfBounds(int width, int height) const {
    return (m_x < 0 || m_x >= width || m_y < 0 || m_y >= height);
}

// Logs a message if the bullet is out of bounds
void Bullet::LogOutOfBounds(int width, int height) const {
    if (IsOutOfBounds(width, height)) {
        std::cout << "Bullet out of bounds at position: (" << m_x << "," << m_y << ")\n";
    }
}

// Detects collision with a player
bool Bullet::DetectCollision(const Player& player) const {
    int playerX, playerY;
    player.GetPosition(playerX, playerY);
    return (m_x == playerX && m_y == playerY);
}
