#include "PlayerEliminatedEvent.h"

std::string PlayerEliminatedEvent::getDescription() const
{
    return "Player " + playerName + " has been eliminated";
}
