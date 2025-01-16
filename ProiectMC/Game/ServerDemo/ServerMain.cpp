//// ServerMain.cpp
//#include "GameServer.h"
//int main() {
//    GameServer server(8080);
//    server.run();
//    return 0;
//}
#include <iostream>
#include "GameLogic.h"

// Crow - server HTTP minimal
//#include "crow_all.h" // Asigură-te că ai crow_all.h în include path

#include "crow.h"
#include <crow/http_request.h>
#include <crow/http_response.h>
#include <crow/app.h>


int main(int argc, char* argv[])
{
    GameLogic game;   // Instanță globală a logicii jocului
    crow::SimpleApp app; // "App" Crow

    // -------------------------
    //    Endpoint: /register
    // -------------------------
    CROW_ROUTE(app, "/register").methods(crow::HTTPMethod::POST)(
        [&](const crow::request& req) {
            auto body = crow::json::load(req.body);

            // Validate the incoming JSON body
            if (!body || !body.has("username") || !body.has("password")) {
                return crow::response(400, "Invalid body");
            }

            // Extract username and password from the JSON body
            std::string username = body["username"].s();
            std::string password = body["password"].s();

            // Call the `doRegister` method to handle the registration logic
            bool ok = game.doRegister(username, password);

            // Return appropriate responses based on the registration outcome
            if (ok) {
                return crow::response(200, "Register success");
            }
            return crow::response(403, "Register fail");
        }
        );



    // -------------------------
    //    Endpoint: /login
    // -------------------------
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
            if (ok)
            {
                return crow::response(200, "Login success");
            }
            return crow::response(403, "Login fail");
        }
        );

    // -------------------------
    //    Endpoint: /startGame
    // -------------------------
    CROW_ROUTE(app, "/startGame").methods(crow::HTTPMethod::POST)(
        [&](const crow::request& req) {
            // Poate n-ai nevoie de body, doar de un token 
            bool ok = game.doStartGame();
            if (ok) return crow::response(200, "Game started");
            return crow::response(403, "StartGame fail");
        }
        );

    // -------------------------
    //    Endpoint: /move
    // -------------------------
    CROW_ROUTE(app, "/move").methods(crow::HTTPMethod::POST)(
        [&](const crow::request& req) {
            auto body = crow::json::load(req.body);
            if (!body.has("username") || !body.has("dx") || !body.has("dy"))
            {
                return crow::response(400, "Invalid body");
            }
            std::string username = body["username"].s();
            int dx = body["dx"].i();
            int dy = body["dy"].i();
            bool ok = game.movePlayer(username, dx, dy);
            if (ok)
                return crow::response(200, "Move success");
            return crow::response(403, "Move fail");
        }
        );

    // -------------------------
    //    Endpoint: /shoot
    // -------------------------
    CROW_ROUTE(app, "/shoot").methods(crow::HTTPMethod::POST)(
        [&](const crow::request& req) {
            auto body = crow::json::load(req.body);
            if (!body.has("username"))
            {
                return crow::response(400, "Invalid body");
            }
            std::string username = body["username"].s();
            game.shoot(username);
            return crow::response(200, "Shoot success");
        }
        );

    // -------------------------
    //    Endpoint: /gameState
    // -------------------------
    // Returnează harta și jucătorii sub formă de JSON
    CROW_ROUTE(app, "/gameState").methods(crow::HTTPMethod::GET)([&]() {
        crow::json::wvalue jsonResp;

        jsonResp["width"] = game.getMapWidth();
        jsonResp["height"] = game.getMapHeight();

        // Serializează celulele
        crow::json::wvalue cellsJson;
        for (int row = 0; row < game.getMapHeight(); ++row) {
            crow::json::wvalue rowJson;
            for (int col = 0; col < game.getMapWidth(); ++col) {
                auto cellType = game.getCell(row, col);
                switch (cellType) {
                case GameLogic::CellType::FREE:
                    rowJson[col] = "FREE";
                    break;
                case GameLogic::CellType::BREAKABLE:
                    rowJson[col] = "BREAKABLE";
                    break;
                case GameLogic::CellType::UNBREAKABLE:
                    rowJson[col] = "UNBREAKABLE";
                    break;
                }
            }
            cellsJson[row] = std::move(rowJson);
        }
        jsonResp["cells"] = std::move(cellsJson);

        // Serializează jucătorii
        auto players = game.getAllPlayers();
        crow::json::wvalue playersJson;

        for (size_t i = 0; i < players.size(); i++) {
            crow::json::wvalue p;
            p["username"] = players[i].getName();
            p["x"] = players[i].getX();
            p["y"] = players[i].getY();
            playersJson[i] = std::move(p);
        }
        jsonResp["players"] = std::move(playersJson);

        return crow::response(200, jsonResp);
        });




    // Pornim serverul pe port 18080 (exemplu)
    app.port(18080).multithreaded().run();

    return 0;
}
