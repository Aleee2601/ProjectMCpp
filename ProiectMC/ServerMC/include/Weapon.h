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
    Weapon();

    void Fire(int startX, int startY, Direction direction); // Lansează un glonț
    void Update(float deltaTime, Map& map);                // Actualizează starea gloanțelor
    void UpgradeCooldown();                                // Reduce timpul de cooldown
    void IncreaseDamage(int value);                        // Crește dauna armelor

    void DisplayWeaponStats() const; // Afișează statisticile armei
};
