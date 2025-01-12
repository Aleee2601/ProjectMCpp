#include "GameDAO.h"
#include "SQLLiteDataBase.cpp"
#include <sqlite_orm/sqlite_orm.h>

using namespace sqlite_orm;
using namespace std;
using namespace sqlite_orm;

DBGame GameDAO::createGame(vector<DBPlayerWithRegion> players) {
    DBGame game;
    // game.game_start = 
    auto storage = initDatabase();
    game.game_id = storage.insert(game);
    for (DBPlayerWithRegion player : players) {
        DBGamePlayer gamePlayer;
        gamePlayer.player_id = player.player_id;
        gamePlayer.region_id = player.region_id;
        gamePlayer.game_id = game.game_id;
        gamePlayer.player_points = 0;
        storage.insert(gamePlayer);
    }
    return game;
}


void GameDAO::updateEndedGame(DBGame game, std::vector<DBPlayerWithRegion> players) {
    auto storage = initDatabase();
    game.setEndTime();
    storage.update(game);

    for (DBPlayerWithRegion player : players) {

        // actualizam punctele fiecarui user din jocul curent
        storage.update_all(sqlite_orm::set(
            c(&DBGamePlayer::player_points) = player.points),
            where(
                and_(
                    is_equal(&DBGamePlayer::game_id, game.game_id),
                    is_equal(&DBGamePlayer::player_id, player.player_id)
                )));

        // adaugam la punctele totale al userului punctele din jocul curent
        storage.update_all(sqlite_orm::set(
            c(&DBPlayer::points) = c(&DBPlayer::points) + player.points),
            where(
                is_equal(&DBPlayer::player_id, player.player_id)
                ));

    }
}