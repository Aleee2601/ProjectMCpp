#include <iostream>
#include <thread>
#include "GameLogic.h"

// Crow - server HTTP minimal
#include "crow.h"
#include <crow/http_request.h>
#include <crow/http_response.h>
#include <crow/app.h>

int main(int argc, char* argv[])
{
    GameLogic game;   // Instanță globală a logicii jocului
    crow::SimpleApp app; // "App" Crow

    // Thread pentru distribuirea periodică a jucătorilor
    std::thread([&]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            game.distributePlayers();
        }
        }).detach();

    // Endpoint: /register
    CROW_ROUTE(app, "/register").methods(crow::HTTPMethod::POST)(
        [&](const crow::request& req) {
            auto body = crow::json::load(req.body);
            if (!body || !body.has("username") || !body.has("password"))
            {
                return crow::response(400, "Invalid body");
            }

            std::string username = body["username"].s();
            std::string password = body["password"].s();
            bool ok = game.doRegister(username, password);
            if (ok) {
                return crow::response(200, "Register success");
            }
            return crow::response(403, "Register fail");
        });

    // Endpoint: /login
    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST)(
        [&](const crow::request& req) {
            auto body = crow::json::load(req.body);
            if (!body || !body.has("username") || !body.has("password"))
            {
                return crow::response(400, "Invalid body");
            }

            std::string username = body["username"].s();
            std::string password = body["password"].s();
            bool ok = game.doLogin(username, password);
            if (ok) {
                return crow::response(200, "Login success");
            }
            return crow::response(403, "Login fail");
        });

    // Endpoint: /startGame
    CROW_ROUTE(app, "/startGame").methods(crow::HTTPMethod::POST)(
        [&](const crow::request& req) {
            auto body = crow::json::load(req.body);
            if (!body.has("username") || !body.has("score")) {
                return crow::response(400, "Invalid request body");
            }

            std::string username = body["username"].s();
            int score = body["score"].i();

            // Adaugă jucătorul în lista de așteptare
            game.addPlayerToWaitingList(username, score);

            // Distribuie jucătorii în jocuri
            game.distributePlayers();

            return crow::response(200, "Player added to waiting list");
        });

    // Endpoint: /gameState
    CROW_ROUTE(app, "/gameState").methods(crow::HTTPMethod::GET)(
        [&](const crow::request& req) {
            crow::json::wvalue jsonResp;

            // Extrage `gameId` din query string (dacă există)
            auto query = crow::query_string(req.url_params);
            auto gameIdStr = query.get("gameId");

            if (gameIdStr) {
                // Dacă `gameId` este prezent, procesează pentru un joc specific
                int gameId = std::stoi(gameIdStr);
                if (gameId < 0 || gameId >= game.getActiveGames().size()) {
                    return crow::response(404, "Invalid game ID");
                }

                // Generează starea pentru jocul specific
                auto& selectedGame = game.getActiveGames()[gameId];

                // Serializează jucătorii
                crow::json::wvalue playersJson;
                for (const auto& player : selectedGame.players) {
                    try {
                        const Player* playerInfo = game.getPlayerInfo(player); // Declare a plain pointer
                        if (playerInfo) { // Ensure the pointer is valid
                            crow::json::wvalue playerJson;
                            playerJson["username"] = playerInfo->getName(); // Access members using ->
                            playerJson["x"] = playerInfo->getX();
                            playerJson["y"] = playerInfo->getY();
                        }
                        else {
                            std::cerr << "[Server] Player info returned null for player: " << player << std::endl;
                        }
                    }
                    catch (const std::exception& e) {
                        std::cerr << "[Server] Error fetching player info: " << e.what() << std::endl;
                    }

                }
                jsonResp["players"] = std::move(playersJson);
                jsonResp["gameId"] = gameId;
            }
            else {
                // Returnează starea implicită
                return crow::response(400, "gameId is required");
            }

            return crow::response(200, jsonResp);
        });

    // Endpoint: /gameInfo
    CROW_ROUTE(app, "/gameInfo").methods(crow::HTTPMethod::GET)(
        [&](const crow::request& req) {
            auto query = crow::query_string(req.url_params);
            auto username = query.get("username");
            if (!username) {
                return crow::response(400, "Username required");
            }

            // Găsește jocul în care este alocat jucătorul
            for (size_t i = 0; i < game.getActiveGames().size(); ++i) {
                auto& activeGame = game.getActiveGames()[i];
                if (std::find(activeGame.players.begin(), activeGame.players.end(), username) != activeGame.players.end()) {
                    crow::json::wvalue resp;
                    resp["gameId"] = i; // Index-ul jocului ca ID
                    return crow::response(200, resp);
                }
            }

            return crow::response(404, "Player not assigned to any game");
        });

    CROW_ROUTE(app, "/activeGames").methods(crow::HTTPMethod::GET)(
        [&](const crow::request& req) {
            crow::json::wvalue jsonResp;

            // Creează o listă JSON (array) pentru jocuri
            jsonResp["games"] = crow::json::wvalue::list();

            for (size_t i = 0; i < game.getActiveGames().size(); ++i) {
                const auto& gameSession = game.getActiveGames()[i];

                // Creează un obiect JSON pentru fiecare joc
                crow::json::wvalue gameJson;
                gameJson["gameId"] = static_cast<int>(i);
                gameJson["players"] = crow::json::wvalue::list(); // Creează o listă JSON pentru jucători

                // Adaugă fiecare jucător în lista de jucători
                size_t playerIndex = 0;
                for (const auto& player : gameSession.players) {
                    gameJson["players"][playerIndex++] = player; // Adaugă jucătorul folosind index
                }

                // Adaugă jocul la lista de jocuri
                jsonResp["games"][i] = std::move(gameJson);
            }

            return crow::response(200, jsonResp);
        });

    CROW_ROUTE(app, "/lobbyPlayers").methods(crow::HTTPMethod::GET)(
        [&](const crow::request& req) {
            // Extrage `gameId` din query string
            auto query = crow::query_string(req.url_params);
            auto gameIdStr = query.get("gameId");

            if (!gameIdStr) {
                return crow::response(400, "gameId is required");
            }

            int gameId = std::stoi(gameIdStr);

            // Verifică dacă gameId este valid
            const auto& activeGames = game.getActiveGames();
            if (gameId < 0 || static_cast<size_t>(gameId) >= activeGames.size()) {
                return crow::response(404, "Invalid gameId");
            }

            // Obține lista de jucători pentru jocul specific
            const auto& selectedGame = activeGames[gameId];
            crow::json::wvalue jsonResp;
            jsonResp["players"] = crow::json::wvalue::list();

            size_t index = 0;
            for (const auto& player : selectedGame.players) {
                jsonResp["players"][index++] = player;
            }

            return crow::response(200, jsonResp);
        });




    // Pornim serverul pe port 18080
    app.port(18080).multithreaded().run();

    return 0;
}
