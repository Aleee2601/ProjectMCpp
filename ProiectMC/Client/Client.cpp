#include <iostream>
#include <regex>
#include <string>
#include <cpr/cpr.h>
#include <crow.h>

void listPlayers() {
    auto response = cpr::Get(cpr::Url{ "http://localhost:8080/get_players" });
    if (response.status_code == 200) {
        std::cout << "Here is the list of players:\n";

        auto players = crow::json::load(response.text);

        if (!players) {
            std::cerr << "Error parsing JSON from response.\n";
            return;
        }

        for (const auto& player : players["players"]) {  // Accesam lista "players" din JSON
            std::cout << "ID: " << player["id"] << '\t'
                << "Name: " << player["name"].s() << '\t'
                << "Position: (" << player["x"] << ", " << player["y"] << ")\n";
        }

    }
    else {
        std::cerr << "Error fetching players. Status code: " << response.status_code << '\n';
    }
}
void addPlayer() {
    int id, x, y;
    std::string name;

    std::cout << "Enter player ID: ";
    std::cin >> id;
    std::cout << "Enter player name: ";
    std::cin >> name;
    std::cout << "Enter X position: ";
    std::cin >> x;
    std::cout << "Enter Y position: ";
    std::cin >> y;

    auto response = cpr::Post(
        cpr::Url{ "http://localhost:8080/add_player" },
        cpr::Body{ "{\"id\":" + std::to_string(id) +
                  ",\"name\":\"" + name +
                  "\",\"x\":" + std::to_string(x) +
                  ",\"y\":" + std::to_string(y) + "}" },
        cpr::Header{ {"Content-Type", "application/json"} }
    );

    if (response.status_code == 200) {
        std::cout << "Player added successfully!\n";
    }
    else {
        std::cerr << "Error adding player. Status code: " << response.status_code << '\n';
    }
}

void updatePlayerPosition() {
    int id, x, y;

    std::cout << "Enter player ID: ";
    std::cin >> id;
    std::cout << "Enter new X position: ";
    std::cin >> x;
    std::cout << "Enter new Y position: ";
    std::cin >> y;

    auto response = cpr::Put(
        cpr::Url{ "http://localhost:8080/update_position" },
        cpr::Body{ "{\"id\":" + std::to_string(id) +
                  ",\"x\":" + std::to_string(x) +
                  ",\"y\":" + std::to_string(y) + "}" },
        cpr::Header{ {"Content-Type", "application/json"} }
    );

    if (response.status_code == 200) {
        std::cout << "Player position updated successfully!\n";
    }
    else {
        std::cerr << "Error updating player position. Status code: " << response.status_code << '\n';
    }
}
