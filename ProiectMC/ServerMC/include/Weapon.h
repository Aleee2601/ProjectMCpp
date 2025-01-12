#pragma once
#include "../include/Bullet.h"
#include <vector>
#include <string>

class Weapon {
private:
    float m_CooldownTime;    // Timpul dintre două focuri consecutive
    float m_BulletSpeed;     // Viteza gloanțelor trase
    int m_Damage;            // Daunele cauzate de gloanțe
    std::vector<Bullet> m_Bullets; // Gloanțele active trase de această armă
    std::string m_Type;      // Tipul armei (ex: Pistol)

public:
    Weapon(float cooldown, float bulletSpeed, int damage, const std::string& type = "Pistol");

    // Funcții legate de gloante
    void Fire(int startX, int startY, Direction direction); // Trage un glonț
    void UpdateBullets(Map& gameMap); // Actualizează poziția gloanțelor

    // Upgrade-uri
    void UpgradeCooldown();
    void IncreaseDamage(int value);

    // Getteri și setteri
    float GetCooldownTime() const;
    void SetCooldownTime(float cooldown);

    float GetBulletSpeed() const;
    void SetBulletSpeed(float speed);

    int GetDamage() const;
    void SetDamage(int value);

    // Statistici
    void DisplayWeaponStats() const;
};
