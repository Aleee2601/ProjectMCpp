#include "../include/GameBuildingDAO.h"
#include "SQLLiteDataBase.cpp"
#include <sqlite_orm/sqlite_orm.h>

DBGameBuilding GameBuildingDAO::insertRecord(int game_id, int building_id, int player_id) {
    DBGameBuilding gameBuilding(game_id, building_id, player_id);
    auto storage = initDatabase();
    gameBuilding.game_building_id = storage.insert(gameBuilding);
    return gameBuilding;
}

DBGameBuilding GameBuildingDAO::insertRecord(DBGame game, DBBuilding building, DBPlayer player)
{
    return insertRecord(game.game_id, building.building_id, player.player_id);
}
