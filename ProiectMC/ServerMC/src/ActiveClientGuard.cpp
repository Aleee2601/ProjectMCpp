#include "include/ActiveClientGuard.h"
#include "Player.h"
#include <iostream>

// Constructor implicit
ActiveClientGuard::ActiveClientGuard() {
    // Initializare, daca este necesar
}

// Destructor
ActiveClientGuard::~ActiveClientGuard() {
    activePlayers.clear();
}

// Adaugare jucator activ
void ActiveClientGuard::addActivePlayer(const Player& player) {
    activePlayers.push_back(player);
    std::cout << "Player added: " << player.GetName() << std::endl; // presupunem metoda getName()
}

// Stergerea unui jucator activ dupa ID
bool ActiveClientGuard::removeActivePlayer(int playerId) {
    for (auto it = activePlayers.begin(); it != activePlayers.end(); ++it) {
        if (it->GetId() == playerId) { // presupunem metoda getId()
            std::cout << "Removing player: " << it->GetName() << std::endl;
            activePlayers.erase(it);
            return true;
        }
    }
    std::cout << "Player with ID " << playerId << " not found." << std::endl;
    return false;
}

// Verificarea activitatii unui jucator
bool ActiveClientGuard::isPlayerActive(int playerId) const {
    for (const auto& player : activePlayers) {
        if (player.GetId() == playerId) {
            return true;
        }
    }
    return false;
}

// Afisarea listei de jucatori activi
void ActiveClientGuard::displayActivePlayers() const {
    std::cout << "Active Players:" << std::endl;
    for (const auto& player : activePlayers) {
        std::cout << "Player ID: " << player.GetId()
            << ", Name: " << player.GetName() << std::endl;
    }
}
