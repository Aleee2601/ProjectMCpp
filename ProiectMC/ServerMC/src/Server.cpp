#include "../include/Server.h"
#include "../include/GameSession.h"

Server::Server() {}

void Server::startServer(uint16_t port) {
    CROW_ROUTE(app, "/move/<int>/<string>")([this](int playerId, std::string direction) {
        handleMoveCommand(playerId, direction);
        return crow::response(200, "Move handled");
        });

    CROW_ROUTE(app, "/position/<int>")([this](int playerId) {
        broadcastPlayerPosition(playerId);
        return crow::response(200, "Position broadcasted");
        });

    CROW_ROUTE(app, "/score/<int>")([this](int playerId) {
        sendScoreUpdate(playerId);
        return crow::response(200, "Score updated");
        });

    app.port(port).multithreaded().run();
}

void Server::handleMoveCommand(int playerId, const std::string& direction) {
    // Logica pentru a gestiona mișcarea unui jucător
    gameSession.MovePlayer(playerId, direction);
}

void Server::broadcastPlayerPosition(int playerId) {
    // Logica pentru a trimite poziția unui jucător
    auto position = gameSession.GetPlayerPosition(playerId);
    CROW_LOG_INFO << "Player " << playerId << " position: " << position.first << ", " << position.second;
}

void Server::sendScoreUpdate(int playerId) {
    // Logica pentru actualizarea scorului
    int score = gameSession.GetPlayerScore(playerId);
    scores[playerId] = score;
    CROW_LOG_INFO << "Player " << playerId << " score: " << score;
}

#include "../include/Server.h"
#include <iostream>

int main() {
    Server server;
    uint16_t port = 18080; // Portul specificat în endpoint-urile tale
    std::cout << "Starting server on port " << port << "...\n";
    server.startServer(port);
    return 0;
}
