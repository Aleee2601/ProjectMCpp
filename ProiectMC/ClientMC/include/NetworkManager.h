#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

class NetworkManager {
public:
    // Constructor
    NetworkManager(const std::string& baseUrl);

    // Send a GET request
    nlohmann::json sendGetRequest(const std::string& endpoint);

    // Send a POST request with JSON body
    nlohmann::json sendPostRequest(const std::string& endpoint, const nlohmann::json& body);

private:
    std::string m_baseUrl;
};
