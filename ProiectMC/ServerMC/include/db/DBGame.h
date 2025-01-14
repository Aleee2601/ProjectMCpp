#pragma once
#include <string>

class DBGame
{
public:
    int game_id;
    std::string game_start;
    std::string game_end;
    int game_region_win_id;

    DBGame();
    void setEndTime();
};
