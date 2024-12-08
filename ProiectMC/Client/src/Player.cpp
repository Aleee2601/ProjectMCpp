#include "../include/map.h"
#include <iostream>
#include <string>


// Constructor: Initializes the player with an ID, name, and starting position
Player::Player(int id, const std::string& name, int startX, int startY)
    : m_id(id), m_name(name), m_x(startX), m_y(startY), m_score(0), m_status(PlayerStatus::ACTIVE), m_hitsTaken(0), m_weapon() {
}

// Sets the player's position
void Player::SetPosition(int newX, int newY) {
    m_x = newX;
    m_y = newY;
}

// Adds points to the player's score
void Player::AddScore(int points) {
    m_score += points;
}

// Returns the player's current score
int Player::GetScore() const {
    return m_score;
}

// Retrieves the player's current position
void Player::GetPosition(int& outX, int& outY) const {
    outX = m_x;
    outY = m_y;
}

// Displays the player's current status
void Player::DisplayStatus() const {
    std::cout << "Player " << m_id << " (" << m_name << ") - Position: (" << m_x << ", " << m_y
        << "), Score: " << m_score << std::endl;
}

// Sets the player's status (ACTIVE or ELIMINATED)
void Player::SetStatus(PlayerStatus newStatus) {
    m_status = newStatus;
}

// Returns the player's current status
PlayerStatus Player::GetStatus() const {
    return m_status;
}

// Resets the player's position to the starting coordinates
void Player::ResetPosition(int startX, int startY) {
    m_x = startX;
    m_y = startY;
}

// Registers a hit on the player and updates their status if necessary
void Player::TakeHit() {
    if (m_status == PlayerStatus::ACTIVE) {
        m_hitsTaken++;
    }
}

// Checks if the player is eliminated
bool Player::IsEliminated() const {
    return m_hitsTaken >= MAX_HITS;
}

// Adds score for hitting another player
void Player::AddScoreForHit() {
    if (m_status == PlayerStatus::ACTIVE) {
        m_score += 100;
    }
}

// Upgrades the player's weapon if their score is sufficient
void Player::UpgradeWeapon() {
    if (m_score >= SCORE_FOR_UPGRADE) {
        m_weapon.UpgradeCooldown();
        std::cout << "Player " << m_name << "'s weapon has been upgraded! Cooldown reduced." << std::endl;
        m_score -= SCORE_FOR_UPGRADE;
    }
}

// Applies a power-up to the player, enhancing their weapon
void Player::ApplyPowerUp() {
    std::cout << "Player " << m_name << " has collected a power-up!" << std::endl;
    m_weapon.UpgradeCooldown();
    m_weapon.IncreaseDamage(5);
}

// Returns the player's ID
int Player::GetId() const {
    return m_id;
}

// Returns the player's x-coordinate
int Player::GetX() const {
    return m_x;
}

// Returns the player's y-coordinate
int Player::GetY() const {
    return m_y;
}
