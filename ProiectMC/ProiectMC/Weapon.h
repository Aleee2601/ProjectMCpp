#pragma once

class Weapon {
private:
    float cooldownTime;
    float bulletSpeed;

public:
    Weapon(float cooldown = 4.0f, float speed = 0.25f)
        : cooldownTime(cooldown), bulletSpeed(speed) {}

    void upgradeCooldown(); // Doar declaram functia aici, fara implementare
    void increaseDamage(int value);
};


