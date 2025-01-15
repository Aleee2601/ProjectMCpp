#include "../include/DBGamePlayer.h"

DBGamePlayer::DBGamePlayer() : game_player_id(0), player_id(0), game_id(0), region_id(0), player_points(0) {}


DBGamePlayer::DBGamePlayer(int gamePlayerId, int playerId, int gameId, int regionId, int playerPoints)
    : game_player_id(gamePlayerId), player_id(playerId), game_id(gameId), region_id(regionId), player_points(playerPoints) {}


void DBGamePlayer::setGamePlayer(int gamePlayerId, int playerId, int gameId, int regionId, int playerPoints) {
    game_player_id = gamePlayerId;
    player_id = playerId;
    game_id = gameId;
    region_id = regionId;
    player_points = playerPoints;
}
