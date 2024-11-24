#pragma once
#include "Event.h"
#include<string>
class PowerUpEvent : public Event {
    std::string playerName;

public:
    PowerUpEvent(const std::string& playerName) : playerName(playerName) {}
    std::string getDescription() const override {
        return "Player " + playerName + " has collected a power-up";
    }
};