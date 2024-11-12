import weapon;  // importam modulul `weapon`

// implementarea functiei upgradeCooldown
void Weapon::upgradeCooldown()
{
    if (cooldownTime > 1.0f)
        cooldownTime /= 2;
}
