#include "Weapon.h"

void Weapon::upgradeCooldown() {
    if (cooldownTime > 1.0f)
        cooldownTime /= 2;
}
