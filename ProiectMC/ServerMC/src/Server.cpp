﻿#include "server.h"

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
    gameSession.movePlayer(playerId, direction);
}

void Server::broadcastPlayerPosition(int playerId) {
    // Logica pentru a trimite poziția unui jucător
    auto position = gameSession.getPlayerPosition(playerId);
    CROW_LOG_INFO << "Player " << playerId << " position: " << position.x << ", " << position.y;
}

void Server::sendScoreUpdate(int playerId) {
    // Logica pentru actualizarea scorului
    int score = gameSession.getPlayerScore(playerId);
    scores[playerId] = score;
    CROW_LOG_INFO << "Player " << playerId << " score: " << score;
}
