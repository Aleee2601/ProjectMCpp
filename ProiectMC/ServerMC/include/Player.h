#pragma once
#include <string>  // Pentru std::string
#include <optional>
#include "Weapon.h"

import direction; // Include pentru enum-ul Direction

// Enum pentru statusul jucătorului
enum class PlayerStatus { ACTIVE, ELIMINATED };

const int MAX_HITS = 3;          // Număr maxim de lovituri pe care le poate primi un jucător
class Player {
private:
    int m_id;  
    int m_x, m_y;                 // Poziția curentă a jucătorului
    int m_startX, m_startY;       // Pozitia de start
    int m_score;   
    int m_kills;
    int m_lives;
    std::string m_imagePath;
    std::string m_name;           // Numele jucătorului
    PlayerStatus m_status;        // Statusul curent al jucătorului (ACTIVE sau ELIMINATED)
    int m_hitsTaken;              // Numărul de lovituri primite de jucător
    Weapon m_weapon;              // Arma jucătorului
    Direction m_direction;        // Direcția curentă a jucătorului
    int m_cooldownUpgrades = 0; // Numărul de îmbunătățiri aplicate cooldown-ului
    int m_winScore = 0;  // Scor obținut pe baza câștigurilor
    bool m_speedBonusUsed = false; // Indică dacă bonusul de viteză a fost aplicat
    std::optional<int> m_teamId; // ID-ul echipei: absent în mod competitiv
    
public:
    /*Player(int id, const std::string& name, int startX, int startY, Direction startDirection = Direction::UP)
        : m_id(id), m_name(name), m_x(startX), m_y(startY), m_score(0), m_status(PlayerStatus::ACTIVE),
        m_hitsTaken(0), m_weapon(), m_direction(startDirection) {
    }*/

    //Player(int id, const std::string& name, int startX, int startY, Direction startDirection = Direction::UP);
    Player(int id, const std::string& name, Direction startDirection, const std::string& imagePath);
    Player();
    std::string GetImage() const;
    void SetStartPosition(int startX, int startY);
    // Declarațiile funcțiilor
    void SetPosition(int newX, int newY);
    void AddWinScore(int points);
    int GetWinScore() const;
    void AddScore(int points);
    int GetScore() const;
    void GetPosition(int& outX, int& outY) const;
    void DisplayStatus() const;
    void SetStatus(PlayerStatus newStatus);

    void SetId(int m_id);
    void SetName(const std::string& m_name);
    void SetLives(int m_lives);
    void SetKills(int m_kills);
    void SetScore(int m_score);
    PlayerStatus GetStatus() const;
    void ResetPosition();
    const std::string& GetName() const;
    int GetId() const;
    int GetX() const;
    int GetY() const;
    Direction GetDirection() const ;
    void TakeHit();
    bool IsEliminated() const;
    void AddScoreForHit();
    void UpgradeWeapon();
    void ApplyPowerUp();
    void Shoot(Direction direction, float delta_time);
    void ResetForFriendlyMode();
    void AssignWeapon(const Weapon& weapon);
    const Weapon& GetWeapon() const;
    Weapon& GetWeapon();
    const std::vector<Bullet>& GetBulletsForPlayer() const {
        return m_weapon.GetBullets();  
    }
    void addKill();
    void loseLife();
    bool isAlive()const;
    int GetLives() const;
    int GetKills() const;

    void AssignTeam(int teamId);
    std::optional<int> GetTeamId() const;

    void AwardWinnerBonus();
};
