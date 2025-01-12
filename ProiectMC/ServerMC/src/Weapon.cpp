#include "../include/Weapon.h"
#include <iostream>

// Constructor
Weapon::Weapon(float cooldown, float bulletSpeed, int damage, const std::string& type)
    : m_CooldownTime(cooldown), m_BulletSpeed(bulletSpeed), m_Damage(damage), m_Type(type) {
}

// Trage un glonț
void Weapon::Fire(int startX, int startY, Direction direction) {
    if (m_CooldownTime > 0) {
        m_Bullets.emplace_back(startX, startY, direction, m_Damage);
        std::cout << m_Type << " fired a bullet from (" << startX << ", " << startY << ")!\n";
    }
    else {
        std::cout << m_Type << " is on cooldown!\n";
    }
}

// Actualizează pozițiile gloanțelor
void Weapon::UpdateBullets(Map& gameMap) {
    for (auto it = m_Bullets.begin(); it != m_Bullets.end();) {
        it->Move(gameMap);
        if (it->IsInactive()) {
            it = m_Bullets.erase(it);
        }
        else {
            ++it;
        }
    }
}

// Upgrade-ul cooldown-ului
void Weapon::UpgradeCooldown() {
    const float MIN_COOLDOWN = 1.0f;
    if (m_CooldownTime > MIN_COOLDOWN) {
        m_CooldownTime /= 2;
        if (m_CooldownTime < MIN_COOLDOWN) {
            m_CooldownTime = MIN_COOLDOWN;
        }
        std::cout << "Cooldown time upgraded to " << m_CooldownTime << " seconds.\n";
    }
    else {
        std::cout << "Cooldown time is already at the minimum value.\n";
    }
}

// Creșterea damage-ului și vitezei gloanțului
void Weapon::IncreaseDamage(int value) {
    m_BulletSpeed += value * 0.05f;
    if (m_BulletSpeed > 5.0f) { // Exemplu: limitare la 5.0f units/s
        m_BulletSpeed = 5.0f;
    }

    m_Damage += value;
    if (m_Damage > 100) { // Exemplu: limitare la 100 puncte
        m_Damage = 100;
    }

    std::cout << "Weapon upgraded: Bullet speed is now " << m_BulletSpeed
        << " units/s, Damage is now " << m_Damage << " points.\n";
}

// Getteri și setteri
float Weapon::GetCooldownTime() const {
    return m_CooldownTime;
}

void Weapon::SetCooldownTime(float cooldown) {
    m_CooldownTime = cooldown;
}

float Weapon::GetBulletSpeed() const {
    return m_BulletSpeed;
}

void Weapon::SetBulletSpeed(float speed) {
    m_BulletSpeed = speed;
}

int Weapon::GetDamage() const {
    return m_Damage;
}

void Weapon::SetDamage(int value) {
    m_Damage = value;
}

// Afișează statisticile armei
void Weapon::DisplayWeaponStats() const {
    std::cout << "Weapon Stats:\n"
        << "Type: " << m_Type << "\n"
        << "Cooldown Time: " << m_CooldownTime << " seconds\n"
        << "Bullet Speed: " << m_BulletSpeed << " units/s\n"
        << "Damage: " << m_Damage << " points\n";
}
