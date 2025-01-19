#pragma once
#include <vector>
#include <iostream>
import bullet;
import direction;

class Weapon {
private:
    std::vector<Bullet> m_bullets; 
    float m_cooldownTime;         
    float m_bulletSpeed;   
    int m_damage;                
    float m_lastFireTime;         

public:
    Weapon() : Weapon(0.0f, 0.25f, 10, 0.0f) {}

    Weapon(float cooldown, float speed, int baseDamage, float lastFireTime);

  
    void FireBullet(int startX, int startY, Direction direction, int ownerId, float delta_time); 
            
    void UpgradeCooldown();                            
    void DisplayWeaponStats() const;
    const std::vector<Bullet>& GetBullets() const {
        return m_bullets;
    }
    std::vector<Bullet>& GetBullets()  {
        return m_bullets;
    }
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

