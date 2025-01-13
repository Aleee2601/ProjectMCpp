#pragma once
#include <string>  // Pentru std::string
#include "Weapon.h"
#include "Map.h"
#include "../include/Direction.h" // Include pentru enum-ul Direction

// Enum pentru statusul jucătorului
enum class PlayerStatus { ACTIVE, ELIMINATED };

const int MAX_HITS = 3;          // Număr maxim de lovituri pe care le poate primi un jucător
const int SCORE_FOR_UPGRADE = 500; // Scor necesar pentru a îmbunătăți arma

class Player {
private:
    int m_id;                     // ID-ul unic al jucătorului
    int m_x, m_y;                 // Poziția curentă a jucătorului
    int m_score;                  // Scorul curent al jucătorului
    std::string m_name;           // Numele jucătorului
    PlayerStatus m_status;        // Statusul curent al jucătorului (ACTIVE sau ELIMINATED)
    int m_hitsTaken;              // Numărul de lovituri primite de jucător
    Weapon m_weapon;              // Arma jucătorului
    Direction m_direction;        // Direcția curentă a jucătorului

public:
    /*Player(int id, const std::string& name, int startX, int startY, Direction startDirection = Direction::UP)
        : m_id(id), m_name(name), m_x(startX), m_y(startY), m_score(0), m_status(PlayerStatus::ACTIVE),
        m_hitsTaken(0), m_weapon(), m_direction(startDirection) {
    }*/

    Player(int id, const std::string& name, int startX, int startY, Direction startDirection = Direction::UP);

    // Declarațiile funcțiilor
    void SetPosition(int newX, int newY);
    void AddScore(int points);
    int GetScore() const;
    void GetPosition(int& outX, int& outY) const;
    void DisplayStatus() const;
    void SetStatus(PlayerStatus newStatus);
    PlayerStatus GetStatus() const;
    void ResetPosition(int startX, int startY);
    std::string GetName() const { return m_name; }
    int GetId() const;
    int GetX() const;
    int GetY() const;
    void Move(Direction direction, const Map& gameMap);
    Direction GetDirection() const { return m_direction; }
    void TakeHit();
    bool IsEliminated() const;
    void AddScoreForHit();
    void UpgradeWeapon();
    void ApplyPowerUp();
    void Shoot(Direction direction); 

};
