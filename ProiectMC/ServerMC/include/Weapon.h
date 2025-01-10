#pragma once
#include <iostream>

class Weapon {
private:
    float m_CooldownTime;  // Time between shots in seconds
    float m_BulletSpeed;   // Speed of the bullets
    int m_Damage;          // Damage dealt by the weapon

public:
    // Constructor: Initializes the weapon with default or specified values
    Weapon(float cooldown = 4.0f, float speed = 0.25f, int baseDamage = 10)
        : m_CooldownTime(cooldown), m_BulletSpeed(speed), m_Damage(baseDamage) {
    }

    // Reduces cooldown time, ensuring it does not fall below a minimum value
    void UpgradeCooldown();

    // Increases bullet speed and weapon damage based on the input value
    void IncreaseDamage(int value);

    // Returns the current cooldown time
    float GetCooldownTime() const;

    // Sets a new cooldown time
    void SetCooldownTime(float cooldown);

    // Returns the current bullet speed
    float GetBulletSpeed() const;

    // Sets a new bullet speed
    void SetBulletSpeed(float speed);

    // Returns the current weapon damage
    int GetDamage() const;

    // Sets a new weapon damage value
    void SetDamage(int value);

    // Displays the current stats of the weapon
    void DisplayWeaponStats() const;
};
