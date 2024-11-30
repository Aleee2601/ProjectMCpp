#include <sqlite_orm/sqlite_orm.h>
#include <iostream>
#include <string>
#include <vector>
#include"DBPlayer.h"

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

//struct Player {
//    int Player_id;
//    std::string Nickname;
//    std::string Password;
//    int Points;
//    int Score;
//};

struct Region {
    int Region_id;
    std::string Region_name;
};


int main() {
    using namespace sqlite_orm;

    auto storage = make_storage("BattleCity.sqlite",
        make_table("Building",
            make_column("Building_id", &Building::Building_id, primary_key()),
            make_column("BuildingRegion_id", &Building::BuildingRegion_id),
            make_column("Building_name", &Building::Building_name)),
        make_table("Game",
            make_column("Game_id", &Game::Game_id, primary_key()),
            make_column("Game_start", &Game::Game_start),
            make_column("Game_end", &Game::Game_end),
            make_column("Game_region_win_id", &Game::Game_region_win_id)),
        make_table("Game_Building",
            make_column("Game_Building_id", &Game_Building::Game_Building_id, primary_key()),
            make_column("Game_id", &Game_Building::Game_id),
            make_column("Building_id", &Game_Building::Building_id),
            make_column("Player_id", &Game_Building::Player_id)),
        make_table("Game_Player",
            make_column("Game_Player_id", &Game_Player::Game_Player_id, primary_key()),
            make_column("Player_id", &Game_Player::Player_id),
            make_column("Game_id", &Game_Player::Game_id),
            make_column("Region_id", &Game_Player::Region_id),
            make_column("Player_points", &Game_Player::Player_points)),
        make_table("Player",
            make_column("Player_id", &DBPlayer::Player_id, primary_key()),
            make_column("Nickname", &DBPlayer::Nickname),
            make_column("Password", &DBPlayer::Password),
            make_column("Points", &DBPlayer::Points),
            make_column("Score", &DBPlayer::Score)),
        make_table("Region",
            make_column("Region_id", &Region::Region_id, primary_key()),
            make_column("Region_name", &Region::Region_name))
    );

    // Creeaza schema tabelelor
    storage.sync_schema();

    // Insereaza date initiale (din fisierul SQL)
    storage.insert(Region{ 1, "Galati" });
    storage.insert(Region{ 2, "Braila" });

    storage.insert(Building{ 1, 1, "Faleza Gl" });
    storage.insert(Building{ 2, 1, "Gradina Botanica" });
    storage.insert(Building{ 3, 1, "Turnul TV" });
    storage.insert(Building{ 4, 1, "Combinatul Siderurgic" });
    storage.insert(Building{ 5, 2, "Faleza Br" });
    storage.insert(Building{ 6, 2, "Teatrul Maria Filotti" });
    storage.insert(Building{ 7, 2, "Port" });
    storage.insert(Building{ 8, 2, "Parcul Monument" });

    // Exemplu de citire din tabel
    auto buildings = storage.get_all<Building>();
    for (const auto& building : buildings) {
        std::cout << "Building: " << building.Building_name << " (Region ID: " << building.BuildingRegion_id << ")\n";
    }

    return 0;
}