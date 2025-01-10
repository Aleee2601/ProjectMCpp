#include "../include/Weapon.h"
#include "../include/Player.h"
#include <iostream>


// Reduces the cooldown time, ensuring it does not go below a minimum value (1.0f)
void Weapon::UpgradeCooldown() {
    const float MIN_COOLDOWN = 1.0f;
    if (m_CooldownTime > MIN_COOLDOWN) {
        m_CooldownTime /= 2;
    }
}

// Increases bullet speed and weapon damage based on the input value
void Weapon::IncreaseDamage(int value) {
    m_BulletSpeed += value * 0.05f; // Increase bullet speed proportionally
    m_Damage += value;             // Increase damage
}

// Returns the current cooldown time
float Weapon::GetCooldownTime() const {
    return m_CooldownTime;
}

// Sets a new cooldown time
void Weapon::SetCooldownTime(float cooldown) {
    m_CooldownTime = cooldown;
}

// Returns the current bullet speed
float Weapon::GetBulletSpeed() const {
    return m_BulletSpeed;
}

// Sets a new bullet speed
void Weapon::SetBulletSpeed(float speed) {
    m_BulletSpeed = speed;
}

// Returns the current weapon damage
int Weapon::GetDamage() const {
    return m_Damage;
}

// Sets a new weapon damage value
void Weapon::SetDamage(int value) {
    m_Damage = value;
}

// Displays the current stats of the weapon
void Weapon::DisplayWeaponStats() const {
    std::cout << "Weapon Stats:\n"
        << "Cooldown Time: " << m_CooldownTime << " seconds\n"
        << "Bullet Speed: " << m_BulletSpeed << " units/s\n"
        << "Damage: " << m_Damage << " points\n";
}
