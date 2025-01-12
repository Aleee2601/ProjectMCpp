#include <sqlite_orm/sqlite_orm.h>
#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
#include <fstream>
#include <filesystem> 
#include "DBPlayer.h"
#include "DBBuilding.h"
#include "DBGame.h"
#include "DBGameBuilding.h"
#include "DBGamePlayer.h"
#include "DBRegion.h"
#include "PlayerDAO.h"

const std::string dataBaseName = "BattleCity.sqlite";

static auto initDatabase()
{
    using namespace sqlite_orm;
    auto storage = make_storage(dataBaseName,
        make_table("Building",
            make_column("Building_id", &DBBuilding::building_id, primary_key()),
            make_column("BuildingRegion_id", &DBBuilding::buildingRegion_id),
            make_column("Building_name", &DBBuilding::building_name)),
        make_table("Game",
            make_column("Game_id", &DBGame::game_id, primary_key()),
            make_column("Game_start", &DBGame::game_start),
            make_column("Game_end", &DBGame::game_end),
            make_column("Game_region_win_id", &DBGame::game_region_win_id)),
        make_table("Game_Building",
            make_column("Game_Building_id", &DBGameBuilding::game_building_id, primary_key()),
            make_column("Game_id", &DBGameBuilding::game_id),
            make_column("Building_id", &DBGameBuilding::building_id),
            make_column("Player_id", &DBGameBuilding::player_id)),
        make_table("Game_Player",
            make_column("Game_Player_id", &DBGamePlayer::game_player_id, primary_key()),
            make_column("Player_id", &DBGamePlayer::player_id),
            make_column("Game_id", &DBGamePlayer::game_id),
            make_column("Region_id", &DBGamePlayer::region_id),
            make_column("Player_points", &DBGamePlayer::player_points)),
        make_table("Player",
            make_column("Player_id", &DBPlayer::player_id, primary_key()),
            make_column("Nickname", &DBPlayer::nickname),
            make_column("Password", &DBPlayer::password),
            make_column("Points", &DBPlayer::points),
            make_column("Score", &DBPlayer::score)),
        make_table("Region",
            make_column("Region_id", &DBRegion::region_id, primary_key()),
            make_column("Region_name", &DBRegion::region_name))
    );

    // Creeaza schema tabelelor
    storage.sync_schema();
    //insertInitialData();
    return storage;
}


static void insertInitialData() {

    auto storage = initDatabase();
    // Insereaza date initiale pentru regiuni
    storage.insert(DBRegion{ 1, "Galati" });
    storage.insert(DBRegion{ 2, "Braila" });

    // Insereaza date initiale pentru elemente cheie
    storage.insert(DBBuilding{ 1, 1, "Faleza Gl" });
    storage.insert(DBBuilding{ 2, 1, "Gradina Botanica" });
    storage.insert(DBBuilding{ 3, 1, "Turnul TV" });
    storage.insert(DBBuilding{ 4, 1, "Combinatul Siderurgic" });
    storage.insert(DBBuilding{ 5, 2, "Faleza Br" });
    storage.insert(DBBuilding{ 6, 2, "Teatrul Maria Filotti" });
    storage.insert(DBBuilding{ 7, 2, "Port" });
    storage.insert(DBBuilding{ 8, 2, "Parcul Monument" });

}