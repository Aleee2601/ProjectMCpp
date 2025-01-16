#include <sqlite_orm/sqlite_orm.h>
#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
#include <fstream>
#include <filesystem> 
#include "../../include/db/DBPlayer.h"
#include "../../include/db/DBGame.h"
#include "../../include/db/DBGamePlayer.h"
#include "../../include/db/DBRegion.h"
#include "../../include/db/PlayerDAO.h"


const std::string dataBaseName = "BattleCity.sqlite";

static auto initDatabase()
{
    using namespace sqlite_orm;
    auto storage = make_storage(dataBaseName,
        make_table("Game",
            make_column("Game_id", &DBGame::game_id, primary_key()),
            make_column("Game_start", &DBGame::game_start),
            make_column("Game_end", &DBGame::game_end),
            make_column("Game_region_win_id", &DBGame::game_region_win_id)),
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


    // doar la crearea bazei de date se introduc o singura data datele initiale
    try {
        DBRegion reggion = storage.get<DBRegion>(1);
    }
    catch (...) {

        std::cout << std::endl << " Introducere date default " << std::endl;
        // Insereaza date initiale pentru regiuni
        storage.insert(DBRegion{ 1, "Galati" });
        storage.insert(DBRegion{ 2, "Braila" });
    };

    return storage;
}






