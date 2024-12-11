#pragma once
#include "../../Client/include/Player.h"  // Include pentru Player
#include "../../Client/include/Map.h"     // Include pentru Map
#include <vector>    // Pentru stocarea jucătorilor activi
#include <string>

class ActiveClientGuard {
public:
    // Constructori si destructor
    ActiveClientGuard();
    ~ActiveClientGuard();

    // Functie pentru adaugarea unui jucator activ
    void addActivePlayer(const Player& player);

    // Functie pentru stergerea unui jucator activ dupa ID
    bool removeActivePlayer(int playerId);

    // Functie pentru verificarea activitatii unui jucator
    bool isPlayerActive(int playerId) const;

    // Afisarea listei de jucatori activi
    void displayActivePlayers() const;

private:
    std::vector<Player> activePlayers;  // Lista jucatorilor activi
};