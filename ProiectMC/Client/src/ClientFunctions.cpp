#include "../include/ClientFunctions.h"
#include <iostream>
#include <cpr/cpr.h>
#include <crow.h>

// Function to add a new player
void ClientFunctions::addPlayer() {
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
        cpr::Url{ "http://localhost:18080/add_player" },
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
        std::cerr << "Error adding player. Status code: " << response.status_code << "\nResponse: " << response.text << "\n";
    }
}

// Function to list all players
void ClientFunctions::listPlayers() {
    auto response = cpr::Get(cpr::Url{ "http://localhost:18080/get_players" });

    if (response.status_code == 200) {
        std::cout << "Players list:\n";

        auto players = crow::json::load(response.text);
        if (!players || !players.has("players")) {
            std::cerr << "Error: Invalid JSON structure received.\n";
            return;
        }

        for (const auto& player : players["players"]) {
            std::cout << "ID: " << player["id"].i() << "\t"
                << "Name: " << player["name"].s() << "\t"
                << "Position: (" << player["x"].i() << ", " << player["y"].i() << ")\n";
        }
    }
    else {
        std::cerr << "Error fetching players. Status code: " << response.status_code << "\nResponse: " << response.text << "\n";
    }
}

// Function to update player position
void ClientFunctions::updatePlayerPosition() {
    int id, x, y;

    std::cout << "Enter player ID: ";
    std::cin >> id;
    std::cout << "Enter new X position: ";
    std::cin >> x;
    std::cout << "Enter new Y position: ";
    std::cin >> y;

    auto response = cpr::Put(
        cpr::Url{ "http://localhost:18080/update_position" },
        cpr::Body{ "{\"id\":" + std::to_string(id) +
                  ",\"x\":" + std::to_string(x) +
                  ",\"y\":" + std::to_string(y) + "}" },
        cpr::Header{ {"Content-Type", "application/json"} }
    );

    if (response.status_code == 200) {
        std::cout << "Player position updated successfully!\n";
    }
    else {
        std::cerr << "Error updating player position. Status code: " << response.status_code << "\nResponse: " << response.text << "\n";
    }
}

// Function to view the map
void ClientFunctions::viewMap() {
    auto response = cpr::Get(cpr::Url{ "http://localhost:18080/map" });

    if (response.status_code == 200) {
        std::cout << "Map:\n";

        auto mapJson = crow::json::load(response.text);
        if (!mapJson || !mapJson.has("grid")) {
            std::cerr << "Error: Invalid map JSON structure.\n";
            return;
        }

        auto grid = mapJson["grid"];
        for (const auto& row : grid) {
            for (const auto& cell : row) {
                std::cout << cell.s() << " ";
            }
            std::cout << "\n";
        }
    }
    else {
        std::cerr << "Error fetching map. Status code: " << response.status_code << "\nResponse: " << response.text << "\n";
    }
}
