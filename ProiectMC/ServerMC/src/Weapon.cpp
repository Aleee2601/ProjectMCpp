#include "../include/Weapon.h"

Weapon::Weapon(float cooldown, float speed, int baseDamage, float lastFireTime)
    : m_cooldownTime(cooldown), m_bulletSpeed(speed), m_damage(baseDamage), m_lastFireTime(lastFireTime) {
}

void Weapon::FireBullet(int startX, int startY, Direction direction, int ownerId, float delta_time) {
    
    m_lastFireTime += delta_time;
    
    if (m_lastFireTime >= m_cooldownTime) {
        int bulletX = startX;
        int bulletY = startY;

        switch (direction) {
        case Direction::UP:
            bulletX -= 1;
            break;
        case Direction::DOWN:
            bulletX += 1; 
            break;
        case Direction::LEFT:
            bulletY -= 1; 
            break;
        case Direction::RIGHT:
            bulletY += 1;
            break;
        }

        std::cout << "Creating bullet at (" << bulletX << ", " << bulletY << ")...\n";
        Bullet newBullet(bulletX, bulletY, direction, ownerId);  
        m_bullets.push_back(newBullet);  
        m_lastFireTime = 0.0f; 
        m_cooldownTime = 4.0f;
        std::cout << "Bullet fired by player " << ownerId << " at (" << startX << ", " << startY
            << ") in direction " << static_cast<int>(direction) << std::endl;
    }
    else {
        std::cout << "Cannot fire: Cooldown active. Time remaining: "
            << (m_cooldownTime - m_lastFireTime) << " seconds\n";
    }
}

void Weapon::UpgradeCooldown() {
    const float MIN_COOLDOWN = 1.0f;
    if (m_cooldownTime > MIN_COOLDOWN) {
        m_cooldownTime /= 2;
    }
}

