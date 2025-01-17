#include "../include/Player.h"
#include "../include/Direction.h" // Include pentru enum-ul Direction
#include <iostream>
#include <string>

// Constructor: Initializes the player with an ID, name, and starting position
//Player::Player(int id, const std::string& name, int startX, int startY, Direction startDirection)
//    : m_id(id), m_name(name), m_startX(startX), m_startY(startY),m_x(startX),m_y(startY),
//    m_score(0), m_status(PlayerStatus::ACTIVE), m_hitsTaken(0), m_weapon(), m_direction(startDirection) {
//}

//Player::Player(int id, const std::string& name, int startX, int startY, Direction startDirection)
//    : m_id(id), m_name(name), m_x(startX), m_y(startY), m_direction(startDirection) {
//}

Player::Player(int id, const std::string& name, int startX, int startY, Direction startDirection, const std::string& imagePath)
    : m_id(id), m_name(name),
    m_x(startX), m_y(startY), // Pozițiile curente
    m_startX(startX), m_startY(startY), // Pozițiile inițiale
    m_direction(startDirection),
    m_status(PlayerStatus::ACTIVE),
    m_hitsTaken(0), m_score(0), m_cooldownUpgrades(0), m_winScore(0), m_speedBonusUsed(false), m_teamId(-1), m_imagePath(imagePath) {
}


// Sets the player's position
void Player::SetPosition(int newX, int newY) {
    m_x = newX;
    m_y = newY;
}
void Player::AddWinScore(int points) {
    m_winScore += points;
    if (m_winScore >= 10 && !m_speedBonusUsed) {
        m_weapon.IncreaseBulletSpeed(0.5f); // Dublează viteza gloanțelor
        m_speedBonusUsed = true;
        m_winScore -= 10; // Scade 10 puncte după aplicarea bonusului
        std::cout << "Speed bonus applied for player " << m_name << "!\n";
    }
}
void Player::AwardWinnerBonus() {
    m_score += 200;
    std::cout << "Player " << m_name << " won the game and received a 200-point bonus!\n";
}
int Player::GetWinScore() const {
    return m_winScore;
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
void Player::ResetPosition() {
    m_x = m_startX;
    m_y = m_startY;
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
    if (m_score >= 500 && m_cooldownUpgrades < 4) {
        m_weapon.UpgradeCooldown();
        m_cooldownUpgrades++;
        std::cout << "Player " << m_name << "'s weapon cooldown upgraded! Cooldown reduced.\n";
        m_score -= 500;
    }
    else if (m_cooldownUpgrades >= 4) {
        std::cout << "Maximum cooldown upgrades reached!\n";
    }
    else {
        std::cout << "Not enough score to upgrade weapon.\n";
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
int Player:: GetY() const {
    return m_y;
}

void Player::Shoot(Direction direction) {
    m_weapon.FireBullet(m_x, m_y, direction,m_id);
}
void Player::ResetForFriendlyMode() {
    m_hitsTaken = 0;
    m_status = PlayerStatus::ACTIVE;
    m_score = 0;          // Resetăm punctele
    m_winScore = 0;       // Resetăm scorul

}

void Player::AssignTeam(int teamId)
{
    m_teamId = teamId;
}

std::optional<int> Player::GetTeamId() const
{
    return m_teamId;
}

std::string Player::GetImage() const {
    return m_imagePath;
}

const std::string& Player::GetName() const {
    return m_name;
}