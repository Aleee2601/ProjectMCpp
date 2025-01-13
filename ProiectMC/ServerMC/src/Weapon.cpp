#include "../include/Weapon.h"


// Trage un glonț în direcția specificată dacă cooldown-ul permite
void Weapon::FireBullet(int startX, int startY, Direction direction) {
    if (m_lastFireTime >= m_cooldownTime) {
        m_bullets.emplace_back(startX, startY, direction); // Creează un glonț nou
        m_lastFireTime = 0.0f; // Resetează timpul ultimei trageri
        std::cout << "Bullet fired at (" << startX << ", " << startY << ") in direction "
            << static_cast<int>(direction) << std::endl;
    }
    else {
        std::cout << "Cannot fire: Cooldown active.\n";
    }
}

// Actualizează starea gloanțelor și elimină gloanțele inactive
void Weapon::UpdateBullets(float deltaTime, Map& map) {
    m_lastFireTime += deltaTime; // Actualizează timpul scurs de la ultima tragere

    // Actualizează fiecare glonț activ
    for (auto& bullet : m_bullets) {
        if (!bullet.IsInactive()) {
            bullet.Move(map); // Mută glonțul pe hartă
        }
    }

    // Elimină gloanțele inactive din vectorul de gloanțe
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

void Weapon::IncreaseDamage(int value) {
    m_damage += value;
    m_bulletSpeed += value * 0.05f; // Crește și viteza gloanțelor
}

void Weapon::DisplayWeaponStats() const {
    std::cout << "Weapon Stats:\n"
        << "Cooldown Time: " << m_cooldownTime << " seconds\n"
        << "Bullet Speed: " << m_bulletSpeed << " units/s\n"
        << "Damage: " << m_damage << " points\n"
        << "Active Bullets: " << m_bullets.size() << "\n";
}

