//#pragma once
//#include "GameSession.h"
//#include "map.h"
//#include <crow.h>
//#include <unordered_map>
//#include <memory>
//
//class Server {
//public:
//    explicit Server();
//    void startServer(uint16_t port);
//
//private:
//    void handleMoveCommand(int playerId, const std::string& direction);
//    void broadcastPlayerPosition(int playerId);
//    void sendScoreUpdate(int playerId);
//
//    crow::SimpleApp app; // Obiect Crow pentru server
//    GameSession gameSession; // Obiect pentru gestionarea sesiunii de joc
//    Map gameMap; // Obiect pentru harta jocului
//    std::unordered_map<int, int> scores; // Mapare între ID-ul jucătorului și scor
//};
#pragma once

#include <crow.h>
#include <map>
#include <mutex>
#include <vector>
#include "GameSession.h"

class Server {
public:
    Server(int port);
    void run();
    void maybeStartGame();

private:
    int m_port;
    crow::SimpleApp m_app;
    std::mutex m_mutex; 
    std::map<std::string, std::string> m_users; // Utilizatori (nume -> informații)
    std::shared_ptr<Map> m_currentMap; // Harta curentă
    std::unique_ptr<GameSession> m_gameSession;
    std::vector<std::unique_ptr<GameSession>> m_activeGames; // Meciuri active
    std::vector<std::string> m_playerQueue; // Coada de jucători
    void initRoutes();
    // Funcții de gestionare
    bool userExists(const std::string& username);
    bool registerUser(const std::string& username);
    void queuePlayer(const std::string& username);
    
};
