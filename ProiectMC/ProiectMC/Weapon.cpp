#include "Weapon.h"

void Weapon::upgradeCooldown() {
    if (cooldownTime > 1.0f)
        cooldownTime /= 2;
}

void Weapon::increaseDamage(int value) {
    bulletSpeed += value * 0.05f;
}
