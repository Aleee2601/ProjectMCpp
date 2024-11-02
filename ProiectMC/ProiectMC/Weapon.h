#pragma once
class Weapon
{
private:
	float cooldownTime;
	float bulletSpeed;
public:
	Weapon(float cooldown=4.0f,float speed=0.25)
		: cooldownTime(cooldown),bulletSpeed(speed) {}

	void upgradeCooldown()
	{
		if (cooldownTime > 1.0f)
			cooldownTime /= 2;

	}
};

