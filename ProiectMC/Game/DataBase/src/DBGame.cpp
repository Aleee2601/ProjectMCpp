#include "../include/DBGame.h"
#include <ctime>

DBGame::DBGame()
{
	game_region_win_id = 0;

	// Obtine timpul curent
	std::time_t now = std::time(nullptr);
	struct tm time_info;
	localtime_s(&time_info, &now);
	char buffer[20];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &time_info);

	// Initializeaza data membru timpul curent
	game_start = buffer;
}

void DBGame::setEndTime()
{
	// Obtine timpul curent
	std::time_t now = std::time(nullptr);
	struct tm time_info;
	localtime_s(&time_info, &now);
	char buffer[20];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &time_info);

	// Initializeaza data membru timpul curent
	game_end = buffer;
}
