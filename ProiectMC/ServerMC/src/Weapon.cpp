﻿#include "../include/Weapon.h"


// Trage un glonț în direcția specificată dacă cooldown-ul permite
void Weapon::FireBullet(int startX, int startY, Direction direction) {
    if (m_lastFireTime >= m_cooldownTime) {
        m_bullets.emplace_back(startX, startY, direction); // Creează un glonț nou
        m_lastFireTime = 0.0f; // Resetează timpul ultimei trageri
        std::cout << "Bullet fired at (" << startX << ", " << startY << ") in direction "
            << static_cast<int>(direction) << std::endl;
    }
    else {
        std::cout << "Cannot fire: Cooldown active. Time remaining: "
            << (m_cooldownTime - m_lastFireTime) << " seconds\n";
    }
}

void Weapon::UpdateBullets(float deltaTime) {
    // Actualizăm timpul scurs de la ultima tragere
    m_lastFireTime += deltaTime;

    // Mutăm gloanțele active
    for (auto& bullet : m_bullets) {
        if (!bullet.IsInactive()) {
            // Calculăm poziția următoare în funcție de direcție
            int nextX = bullet.GetX();
            int nextY = bullet.GetY();

            switch (bullet.GetDirection()) {
            case Direction::UP:
                nextY--;
                break;
            case Direction::DOWN:
                nextY++;
                break;
            case Direction::LEFT:
                nextX--;
                break;
            case Direction::RIGHT:
                nextX++;
                break;
            }
        }
    }

    // Eliminăm gloanțele inactive din vectorul `m_bullets`
    m_bullets.erase(
        std::remove_if(m_bullets.begin(), m_bullets.end(),
            [](const Bullet& b) { return b.IsInactive(); }),
        m_bullets.end());
}


void Weapon::UpgradeCooldown() {
    const float MIN_COOLDOWN = 1.0f;
    if (m_cooldownTime > MIN_COOLDOWN) {
        m_cooldownTime /= 2;
    }
}


