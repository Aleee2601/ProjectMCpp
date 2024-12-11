#pragma once

#include<string>
class Event
{
public:
	virtual std::string getDescription() const = 0;
	virtual ~Event(){}
};

