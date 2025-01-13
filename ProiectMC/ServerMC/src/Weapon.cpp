#include "../include/Weapon.h"


void Weapon::FireBullet(int startX, int startY, Direction direction) {
    if (m_lastFireTime >= m_cooldownTime) {
        m_bullets.emplace_back(startX, startY, direction); // Creează un glonț nou
        m_lastFireTime = 0.0f; // Resetează timpul ultimei trageri
        std::cout << "Bullet fired at (" << startX << ", " << startY << ") in direction " << static_cast<int>(direction) << std::endl;
    }
}

void Weapon::UpdateBullets(float deltaTime, Map& map) {
    m_lastFireTime += deltaTime;

    for (auto& bullet : m_bullets) {
        if (!bullet.IsInactive()) {
            bullet.Move(map);
        }
    }

    m_bullets.erase(
        std::remove_if(m_bullets.begin(), m_bullets.end(), [](const Bullet& b) { return b.IsInactive(); }),
        m_bullets.end());
}

void Weapon::UpgradeCooldown() {
    const float MIN_COOLDOWN = 1.0f;
    if (m_cooldownTime > MIN_COOLDOWN) {
        m_cooldownTime /= 2;
    }
}

void Weapon::IncreaseDamage(int value) {
    m_damage += value;
    m_bulletSpeed += value * 0.05f; // Crește și viteza gloanțelor
}

void Weapon::DisplayWeaponStats() const {
    std::cout << "Weapon Stats:\n"
        << "Cooldown Time: " << m_cooldownTime << " seconds\n"
        << "Bullet Speed: " << m_bulletSpeed << " units/s\n"
        << "Damage: " << m_damage << " points\n";
}
