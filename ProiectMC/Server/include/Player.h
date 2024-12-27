#pragma once
#include <string>
#include "Weapon.h"
#include "map.h"

// Enum for player's status
enum class PlayerStatus { ACTIVE, ELIMINATED };
enum class Direction { UP, DOWN, LEFT, RIGHT };

const int MAX_HITS = 3;          // Maximum hits a player can take before elimination
const int SCORE_FOR_UPGRADE = 500; // Score required to upgrade the weapon

class Player {
private:
    int m_id;                     // Player's unique ID
    int m_x, m_y;                 // Player's current position
    int m_score;                  // Player's current score
    std::string m_name;           // Player's name
    PlayerStatus m_status;        // Player's current status (ACTIVE or ELIMINATED)
    int m_hitsTaken;              // Number of hits the player has taken
    Weapon m_weapon;              // Player's weapon

public:
    // Constructor: Initializes a player with an ID, name, and starting position
    Player(int id, const std::string& name, int startX, int startY);

    // Sets the player's position
    void SetPosition(int newX, int newY);

    // Adds points to the player's score
    void AddScore(int points);

    // Returns the player's current score
    int GetScore() const;

    // Retrieves the player's current position
    void GetPosition(int& outX, int& outY) const;

    // Displays the player's current status
    void DisplayStatus() const;

    // Sets the player's status (ACTIVE or ELIMINATED)
    void SetStatus(PlayerStatus newStatus);

    // Returns the player's current status
    PlayerStatus GetStatus() const;

    // Resets the player's position to the starting coordinates
    void ResetPosition(int startX, int startY);

    // Returns the player's name
    std::string GetName() const { return m_name; }

    // Returns the player's ID
    int GetId() const;

    // Returns the player's x-coordinate
    int GetX() const;

    // Returns the player's y-coordinate
    int GetY() const;

    // Registers a hit on the player and updates their status if necessary
    void TakeHit();

    // Checks if the player is eliminated
    bool IsEliminated() const;

    // Adds score for hitting another player
    void AddScoreForHit();

    // Upgrades the player's weapon
    void UpgradeWeapon();

    // Applies a power-up to the player
    void ApplyPowerUp();

    void Move(Direction direction, const Map& gameMap);
};
