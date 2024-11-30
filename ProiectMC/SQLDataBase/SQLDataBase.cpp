#include <sqlite_orm/sqlite_orm.h>
#include <iostream>
#include <string>
#include <vector>

struct Building {
    int Building_id;
    int BuildingRegion_id;
    std::string Building_name;
};

struct Game {
    int Game_id;
    std::string Game_start;
    std::string Game_end;
    int Game_region_win_id;
};

struct Game_Building {
    int Game_Building_id;
    int Game_id;
    int Building_id;
    int Player_id;
};

struct Game_Player {
    int Game_Player_id;
    int Player_id;
    int Game_id;
    int Region_id;
    int Player_points;
};

struct Player {
    int Player_id;
    std::string Nickname;
    std::string Password;
    int Points;
    int Score;
};

struct Region {
    int Region_id;
    std::string Region_name;
};
