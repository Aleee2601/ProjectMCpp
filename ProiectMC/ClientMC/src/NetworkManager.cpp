#include "../include/NetworkManager.h"
#include <iostream>

NetworkManager::NetworkManager(const std::string& baseUrl)
    : m_baseUrl(baseUrl) {
}

nlohmann::json NetworkManager::sendGetRequest(const std::string& endpoint) {
    try {
        cpr::Response response = cpr::Get(cpr::Url{ m_baseUrl + endpoint });
        if (response.status_code == 200) {
            return nlohmann::json::parse(response.text);
        }
        else {
            std::cerr << "GET request failed: " << response.status_code << " - " << response.text << "\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error in GET request: " << e.what() << "\n";
    }
    return nlohmann::json{};
}

nlohmann::json NetworkManager::sendPostRequest(const std::string& endpoint, const nlohmann::json& body) {
    try {
        cpr::Response response = cpr::Post(
            cpr::Url{ m_baseUrl + endpoint },
            cpr::Body{ body.dump() },
            cpr::Header{ {"Content-Type", "application/json"} }
        );
        if (response.status_code == 200) {
            return nlohmann::json::parse(response.text);
        }
        else {
            std::cerr << "POST request failed: " << response.status_code << " - " << response.text << "\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error in POST request: " << e.what() << "\n";
    }
    return nlohmann::json{};
}
