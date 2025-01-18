﻿#pragma once
#include <vector>
#include <iostream>
import bullet;
import direction;

class Weapon {
private:
    std::vector<Bullet> m_bullets; // Lista gloanțelor trase
    float m_cooldownTime;          // Timpul de cooldown între trageri
    float m_bulletSpeed;           // Viteza gloanțelor
    int m_damage;                  // Dauna produsă de gloanțe
    float m_lastFireTime;          // Timpul ultimei trageri

public:
    // Constructor implicit pentru pistol
    Weapon() : Weapon(4.0f, 0.25f, 10) {}

    // Constructor general pentru arme
    Weapon(float cooldown, float speed, int baseDamage);

    void FireBullet(int startX, int startY, Direction direction, int ownerId); // New
    void UpdateBullets(float deltaTime);                // Actualizează starea gloanțelor
    void UpgradeCooldown();                                // Reduce timpul de cooldown
    void DisplayWeaponStats() const;
    template <typename T>
    void Increase(T value) {
        if (value > 0) {
            if constexpr (std::is_same<T, float>::value) {
                m_bulletSpeed += value;
                std::cout << "Bullet speed increased by " << value
                    << ". New bullet speed: " << m_bulletSpeed << " units/s\n";
            }
            else if constexpr (std::is_same<T, int>::value) {
                m_damage += value;
                std::cout << "Weapon damage increased by " << value
                    << ". New damage: " << m_damage << "\n";
            }
        }
        else {
            std::cout << "Invalid value. No changes made.\n";
        }
    }
    float GetCooldownTime() const { return m_cooldownTime; }
    int GetDamage() const { return m_damage; }
};

