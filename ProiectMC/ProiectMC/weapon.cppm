export module weapon;

export class Weapon
{
private:
    float cooldownTime;
    float bulletSpeed;

public:
    Weapon(float cooldown = 4.0f, float speed = 0.25f)
        : cooldownTime(cooldown), bulletSpeed(speed) {}

    void upgradeCooldown()
    {
        if (cooldownTime > 1.0f)
            cooldownTime /= 2;
    }
};
