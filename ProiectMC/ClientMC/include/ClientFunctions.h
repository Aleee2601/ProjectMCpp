#pragma once
#include <string>
/*
    add player, list players, update player position, view map, reset game
*/

class ClientFunctions {
public:
    // Adds a player
    static void addPlayer();

    // Lists all players
    static void listPlayers();

    // Updates the position of a player
    static void updatePlayerPosition();

    // Displays the map
    static void viewMap();

    // Function to reset the game
    static void resetGame();

    void movePlayer(int playerId, const std::string& direction);
    void getPlayerPosition(int playerId);
    void getPlayerScore(int playerId);

    void getGameState();
    void registerPlayer(int playerId, const std::string& name);
};
