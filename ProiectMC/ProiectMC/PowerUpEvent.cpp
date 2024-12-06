#include "PowerUpEvent.h"

std::string PowerUpEvent::getDescription() const
{
    return "Player " + playerName + " has collected a power-up";
}
