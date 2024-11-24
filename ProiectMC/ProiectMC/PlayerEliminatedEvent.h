#pragma once
#include "Event.h"
class PlayerEliminatedEvent : 
    
public Event {
    std::string playerName;

public:
    PlayerEliminatedEvent(const std::string& playerName) : playerName(playerName) {}
    std::string getDescription() const override {
        return "Player " + playerName + " has been eliminated";
    }


};