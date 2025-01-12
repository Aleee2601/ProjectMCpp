#include "../include/Bullet.h"
#include "../include/map.h"
#include "../include/Player.h"
#include "../include/Direction.h"
#include <iostream>
#include <unordered_map>

// Constructor for a bullet with a starting position and direction
Bullet::Bullet(int startX, int startY, Direction dir)
    : m_x(startX), m_y(startY), m_direction(dir), m_endX(0), m_endY(0), m_Active(true) {
}

// Constructor for a bullet with a starting and ending position, plus direction
Bullet::Bullet(int startX, int startY, int endX, int endY, Direction dir)
    : m_x(startX), m_y(startY), m_endX(endX), m_endY(endY), m_direction(dir), m_Active(true) {
}

// Moves the bullet one step in its direction
void Bullet::Move(Map& map) {
    if (!m_Active) return; // Dacă glonțul nu este activ, nu mai face nimic

    // Determină următoarea poziție în funcție de direcție
    switch (m_direction) {
    case Direction::UP:    --m_y; break;
    case Direction::DOWN:  ++m_y; break;
    case Direction::LEFT:  --m_x; break;
    case Direction::RIGHT: ++m_x; break;
    }

    // Verifică limitele hărții
    if (!map.IsWithinBounds(m_x, m_y)) {
        std::cout << "Bullet went out of bounds at (" << m_x << ", " << m_y << ")\n";
        m_Active = false; // Dezactivează glonțul
        return;
    }

    // Verifică coliziunea cu pereții
    if (map.GetCellType(m_x, m_y) == CellType::DESTRUCTIBLE_WALL) {
        map.SetCellType(m_x, m_y, CellType::EMPTY); // Distruge peretele
        std::cout << "Bullet hit a destructible wall at (" << m_x << ", " << m_y << ")\n";
        m_Active = false; // Dezactivează glonțul
    }
    else if (map.GetCellType(m_x, m_y) == CellType::INDESTRUCTIBLE_WALL) {
        std::cout << "Bullet hit an indestructible wall at (" << m_x << ", " << m_y << ")\n";
        m_Active = false; // Dezactivează glonțul
    }
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

// Checks if the bullet is inactive
bool Bullet::IsInactive() const {
    return !m_Active; // Glonțul este inactiv dacă `m_Active` este fals
}
