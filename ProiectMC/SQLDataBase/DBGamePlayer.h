#pragma once
class DBGamePlayer
{
public:
    int game_player_id;
    int player_id;
    int game_id;
    int region_id;
    int player_points;

    DBGamePlayer();
    DBGamePlayer(int gamePlayerId, int playerId, int gameId, int regionId, int playerPoints);

  
    void setGamePlayer(int gamePlayerId, int playerId, int gameId, int regionId, int playerPoints);

};