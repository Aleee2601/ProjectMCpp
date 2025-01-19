#pragma once
#include <string>
#include "NetworkManager.h"
#include "../ServerMC/include/Player.h"

class ClientFunctions {
private:
    std::string m_serverUrl;
    NetworkManager m_networkManager;

public:

    explicit ClientFunctions(const std::string& serverUrl);
    ClientFunctions(NetworkManager& networkManager);
    ClientFunctions();
    // Adds a player
    static void addPlayer();

    // Lists all players
    static void listPlayers();

    // Updates the position of a player
    static void updatePlayerPosition();

    // Displays the map
    //static void viewMap();

    // Function to reset the game
    static void resetGame();

    void movePlayer(int playerId, const std::string& direction);
    void getPlayerPosition(int playerId);
    void getPlayerScore(int playerId);

    void getGameState();
    void registerPlayer(int playerId, const std::string& name);

    //-------------------------------------------
    //  Din celalalt cod
    //-------------------------------------------
    /*
    bool doStartGameRequest();
    bool sendMoveRequest(int dx, int dy);
    bool sendShootRequest();

    bool getGameInfo();
    */

    bool doRegisterRequest(const std::string& user, const std::string& pass);
    bool doLoginRequest(const std::string& user, const std::string& pass);
    void displayDashboard(const std::vector<Player>& players);
    
    std::vector<Player> listPlayersDashBoard();

        void fetchLobby();
        void startGame();

        void viewMapFunction(NetworkManager & networkManager);
        void updateMapFunction(NetworkManager & networkManager, int x, int y, int newType);
 };
