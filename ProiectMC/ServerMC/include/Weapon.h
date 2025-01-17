#pragma once
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

    void IncreaseBulletSpeed(float value); // Crește viteza gloanțelor
    void FireBullet(int startX, int startY, Direction direction, int ownerId); // New
    void UpdateBullets(float deltaTime);                // Actualizează starea gloanțelor
    void UpgradeCooldown();                                // Reduce timpul de cooldown
    void IncreaseDamage(int value);                        // Crește dauna armelor

    void DisplayWeaponStats() const; // Afișează statisticile armei
};
//hbuygyfvty
