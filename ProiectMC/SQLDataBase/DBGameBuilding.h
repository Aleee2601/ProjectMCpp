#pragma once
class DBGameBuilding
{

public:
    int game_building_id;
    int game_id;
    int building_id;
    int player_id;
    DBGameBuilding(int game_id, int building_id, int player_id);

};