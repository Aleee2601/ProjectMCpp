#include "../include/Weapon.h"

Weapon::Weapon(float cooldown, float speed, int baseDamage, float lastFireTime)
    : m_cooldownTime(cooldown), m_bulletSpeed(speed), m_damage(baseDamage), m_lastFireTime(lastFireTime) {
}

// Trage un glonț în direcția specificată dacă cooldown-ul permite
void Weapon::FireBullet(int startX, int startY, Direction direction, int ownerId) {
    if (m_lastFireTime >= m_cooldownTime) {
        Bullet newBullet(startX, startY, direction, ownerId);  // Crearea obiectului Bullet
        m_bullets.push_back(newBullet);  // Adăugarea obiectului în vectorul de gloanțe
        m_lastFireTime = 0.0f; // Resetează timpul ultimei trageri
        std::cout << "Bullet fired by player " << ownerId << " at (" << startX << ", " << startY
            << ") in direction " << static_cast<int>(direction) << std::endl;
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
                bullet.SetPosition(nextX, nextY);
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

