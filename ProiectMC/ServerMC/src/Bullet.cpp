#include "../include/Bullet.h"
#include <iostream>

Bullet::Bullet(int startX, int startY, Direction dir)
    : m_x(startX), m_y(startY), m_direction(dir), m_active(true) {
}

void Bullet::Move(Map& map) {
    if (!m_active) return;

    switch (m_direction) {
    case Direction::UP:    --m_y; break;
    case Direction::DOWN:  ++m_y; break;
    case Direction::LEFT:  --m_x; break;
    case Direction::RIGHT: ++m_x; break;
    }

    if (!map.IsWithinBounds(m_x, m_y) || map.IsCollisionWithWall(m_x, m_y)) {
        m_active = false;
    }

    std::cout << "Bullet moved to (" << m_x << ", " << m_y << ")\n";
}

bool Bullet::IsInactive() const {
    return !m_active;
}
