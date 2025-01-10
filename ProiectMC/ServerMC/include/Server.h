#pragma once
#include "GameSession.h"
#include "map.h"
#include <crow.h>
#include <unordered_map>
#include <memory>

class Server {
public:
    explicit Server();
    void startServer(uint16_t port);

private:
    void handleMoveCommand(int playerId, const std::string& direction);
    void broadcastPlayerPosition(int playerId);
    void sendScoreUpdate(int playerId);

    crow::SimpleApp app; // Obiect Crow pentru server
    GameSession gameSession; // Obiect pentru gestionarea sesiunii de joc
    Map gameMap; // Obiect pentru harta jocului
    std::unordered_map<int, int> scores; // Mapare între ID-ul jucătorului și scor
};
