#pragma once
#include "Event.h"


class GameEndEvent : public Event {
public:
	std::string getDescription() const override {
		return "The game has ended";
	}
};