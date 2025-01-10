#pragma once
#include <string>

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
};
