#include "../include/ClientFunctions.h"
#include <iostream>
#include <cpr/cpr.h>
#include <stdexcept>
#include <crow.h>
#include <crow/json.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

ClientFunctions::ClientFunctions(const std::string& serverUrl)
    : m_networkManager(serverUrl) {
}
ClientFunctions::ClientFunctions(NetworkManager& networkManager)
    : m_networkManager(networkManager){
}

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

// Resets the game: all players are removed and the map is reset
void ClientFunctions::resetGame() {
    auto response = cpr::Post(cpr::Url{ "http://localhost:18080/reset_game" });

    if (response.status_code == 200) {
        std::cout << "Game has been successfully reset!\n";
    }
    else {
        std::cerr << "Error resetting the game. Status code: " << response.status_code << '\n';
    }
}

void ClientFunctions::movePlayer(int playerId, const std::string& direction) {
    auto response = cpr::Get(cpr::Url{ "http://localhost:18080/move/" + std::to_string(playerId) + "/" + direction });

    if (response.status_code == 200) {
        std::cout << "Player moved successfully.\n";
    }
    else {
        std::cerr << "Error moving player. Status code: " << response.status_code << "\n";
    }
}

void ClientFunctions::getPlayerPosition(int playerId) {
    auto response = cpr::Get(cpr::Url{ "http://localhost:18080/position/" + std::to_string(playerId) });

    if (response.status_code == 200) {
        std::cout << "Player position retrieved successfully.\n";
    }
    else {
        std::cerr << "Error fetching player position. Status code: " << response.status_code << "\n";
    }
}
void ClientFunctions::getPlayerScore(int playerId) {
    auto response = cpr::Get(cpr::Url{ "http://localhost:18080/score/" + std::to_string(playerId) });

    if (response.status_code == 200) {
        std::cout << "Player score updated successfully.\n";
    }
    else {
        std::cerr << "Error updating player score. Status code: " << response.status_code << "\n";
    }
}

//void ClientFunctions::registerPlayer(int playerId, const std::string& name) {
//    auto response = cpr::Get(cpr::Url{ "http://localhost:18080/register_player/" + std::to_string(playerId) + "/" + name });
//
//    if (response.status_code == 200) {
//        std::cout << "Player registered successfully.\n";
//    }
//    else {
//        std::cerr << "Error registering player. Status code: " << response.status_code << "\n";
//    }
//}

void ClientFunctions::getGameState() {
    auto response = cpr::Get(cpr::Url{ "http://localhost:18080/game_state" });

    if (response.status_code == 200) {
        std::cout << "Game state:\n" << response.text << "\n";
    }
    else {
        std::cerr << "Error fetching game state. Status code: " << response.status_code << "\n";
    }
}

// REGISTER
bool ClientFunctions::doRegisterRequest(const std::string& user, const std::string& pass) {
    // Prepare JSON request body
    nlohmann::json requestBody;
    requestBody["username"] = user;
    requestBody["password"] = pass;

    // Send POST request using NetworkManager
    nlohmann::json response = m_networkManager.sendPostRequest("/register", requestBody);

    // Handle the response
    if (!response.empty() && response.contains("message")) {
        std::cout << "Registration successful: " << response["message"] << std::endl;
        return true;
    }
    else if (response.contains("error") && response["error"] == "Username already exists") {
        std::cerr << "Registration failed: Username already exists.\n";
    }
    else {
        std::cerr << "Registration failed: Invalid response from server or error occurred." << std::endl;
    }
    return false;
}

// LOGIN
bool ClientFunctions::doLoginRequest(const std::string& user, const std::string& pass) {
    // Prepare JSON request body
    nlohmann::json requestBody;
    requestBody["username"] = user;
    requestBody["password"] = pass;

    // Send POST request using NetworkManager
    nlohmann::json response = m_networkManager.sendPostRequest("/login", requestBody);

    // Handle the response
    if (!response.empty() && response.contains("message")) {
        std::cout << "Login successful: " << response["message"] << std::endl;
        return true;
    }
    else {
        std::cerr << "Login failed: Invalid response from server or error occurred." << std::endl;
    }
    return false;
}

void ClientFunctions::fetchLobby() {
    auto response = m_networkManager.sendGetRequest("/lobby");
    if (!response.empty()) {
        std::cout << "Lobby data:\n" << response.dump(4) << std::endl;
    }
}

void ClientFunctions::startGame() {
    auto response = m_networkManager.sendPostRequest("/startGame", {});
    if (!response.empty()) {
        std::cout << "Server response: " << response.dump(4) << std::endl;
    }
}


// 
//  MAP
//
void ClientFunctions::viewMapFunction(NetworkManager& networkManager) {
    // Send GET request to /currentMap endpoint
    nlohmann::json responseJson = networkManager.sendGetRequest("/currentMap");

    // Check if the response contains a valid map structure
    if (responseJson.empty() || !responseJson.contains("map")) {
        std::cerr << "Error: Invalid or empty response from server.\n";
        return;
    }

    try {
        std::cout << "Map:\n";

        // Extract and print the map grid
        auto& mapGrid = responseJson["map"];
        for (const auto& row : mapGrid) {
            for (const auto& cell : row) {
                std::cout << cell.get<int>() << " ";
            }
            std::cout << "\n";
        }

        // Optionally print player information if present
        if (responseJson.contains("players")) {
            std::cout << "\nPlayers:\n";
            for (const auto& player : responseJson["players"]) {
                std::cout << "ID: " << player["id"].get<int>()
                    << ", Name: " << player["name"].get<std::string>()
                    << ", Position: (" << player["x"].get<int>() << ", " << player["y"].get<int>() << ")\n";
            }
        }

        // Optionally print bomb information if present
        if (responseJson.contains("bombs")) {
            std::cout << "\nBombs:\n";
            for (const auto& bomb : responseJson["bombs"]) {
                std::cout << "Position: (" << bomb["x"].get<int>() << ", " << bomb["y"].get<int>()
                    << "), Radius: " << bomb["radius"].get<int>() << "\n";
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error processing map data: " << e.what() << "\n";
    }
}

void ClientFunctions::updateMapFunction(NetworkManager& networkManager, int x, int y, int newType) {
    // Prepare the JSON body for the POST request
    nlohmann::json body = {
        {"x", x},
        {"y", y},
        {"type", newType}
    };

    // Send the POST request to /updateMap endpoint
    nlohmann::json responseJson = networkManager.sendPostRequest("/updateMap", body);

    // Check the response for success or failure
    if (!responseJson.empty() && responseJson.contains("status") && responseJson["status"] == "success") {
        std::cout << "Map updated successfully:\n";
        auto& updatedCell = responseJson["updated_cell"];
        std::cout << "Updated Cell: (" << updatedCell["x"].get<int>() << ", " << updatedCell["y"].get<int>()
            << ") New Type: " << updatedCell["new_type"].get<int>() << "\n";
    }
    else {
        std::cerr << "Failed to update the map.\n";
    }
}