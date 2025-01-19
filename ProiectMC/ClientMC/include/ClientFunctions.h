#pragma once
#include <string>
#include "NetworkManager.h"
//#include "../ServerMC/include/Player.h"

class ClientFunctions {
private:
    std::string m_serverUrl;
    NetworkManager m_networkManager;

public:

    explicit ClientFunctions(const std::string& serverUrl);
    ClientFunctions(NetworkManager& networkManager);
    ClientFunctions();
 
    static void addPlayer();

    static void listPlayers();

    static void updatePlayerPosition();

    static void resetGame();

    void movePlayer(int playerId, const std::string& direction);
    void getPlayerPosition(int playerId);
    void getPlayerScore(int playerId);

    bool doRegisterRequest(const std::string& user, const std::string& pass);
    bool doLoginRequest(const std::string& user, const std::string& pass);
    void displayDashboard(NetworkManager& networkManager);
    
    void fetchLobby();
    void startGame();

    void viewMapFunction(NetworkManager & networkManager);
    void updateMapFunction(NetworkManager & networkManager, int x, int y, int newType);
 };
