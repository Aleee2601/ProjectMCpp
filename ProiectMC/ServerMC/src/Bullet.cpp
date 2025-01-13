#include "../include/Bullet.h"
#include "../include/Direction.h""
#include <utility> 
#include <iostream>

void Bullet::Move(Map& map) {
    if (!m_active) return;

    // Calculăm poziția următoare bazată pe direcția glonțului
    auto [nextX, nextY] = GetNextPosition(m_x, m_y, m_direction);

    // Verificăm limitele hărții și coliziunile
    if (!map.IsWithinBounds(nextX, nextY) || map.IsCollisionWithWall(nextX, nextY)) {
        m_active = false; // Glonțul devine inactiv
        std::cout << "Bullet hit a wall at (" << nextX << ", " << nextY << ")\n";
    }
    else if (map.IsPlayer(nextX, nextY)) {
        m_active = false; // Glonțul lovește un jucător
        map.KillPlayerAt(nextX, nextY); // Eliminăm jucătorul de pe hartă
        std::cout << "Bullet hit a player at (" << nextX << ", " << nextY << ")\n";
    }
    else {
        // Actualizăm poziția glonțului
        m_x = nextX;
        m_y = nextY;
        std::cout << "Bullet moved to (" << m_x << ", " << m_y << ")\n";
    }

}
