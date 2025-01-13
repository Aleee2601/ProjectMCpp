#pragma once
#include "Bullet.h"
#include <vector>
#include <iostream>

class Weapon {
private:
    std::vector<Bullet> m_bullets; // Lista gloanțelor trase
    float m_cooldownTime;          // Timpul de cooldown între trageri
    float m_bulletSpeed;           // Viteza gloanțelor
    int m_damage;                  // Dauna produsă de gloanțe
    float m_lastFireTime;          // Timpul ultimei trageri

public:
    //Weapon();
    Weapon(float cooldown = 4.0f, float speed = 0.25f, int baseDamage = 10);
    void FireBullet(int startX, int startY, Direction direction); // New
    void UpdateBullets(float deltaTime, Map& map);                // Actualizează starea gloanțelor
    void UpgradeCooldown();                                // Reduce timpul de cooldown
    void IncreaseDamage(int value);                        // Crește dauna armelor

    void DisplayWeaponStats() const; // Afișează statisticile armei
};
