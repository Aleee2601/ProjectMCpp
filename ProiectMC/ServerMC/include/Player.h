#pragma once
#include <string>  
#include <optional>
#include "Weapon.h"

import direction; 

enum class PlayerStatus { ACTIVE, ELIMINATED };

const int MAX_HITS = 3;     
class Player {
private:
    int m_id;  
    int m_x, m_y;             
    int m_startX, m_startY;      
    int m_score;   
    int m_kills;
    int m_lives;
    std::string m_imagePath;
    std::string m_name;         
    PlayerStatus m_status;       
    int m_hitsTaken;              
    Weapon m_weapon;             
    Direction m_direction;       
    int m_cooldownUpgrades = 0; 
    int m_winScore = 0; 
    bool m_speedBonusUsed = false; 
    std::optional<int> m_teamId; 
    
public:

    Player(int id, const std::string& name, Direction startDirection, const std::string& imagePath);
    std::string GetImage() const;
    void SetStartPosition(int startX, int startY);
    void SetPosition(int newX, int newY);
    void AddWinScore(int points);
    int GetWinScore() const;
    void AddScore(int points);
    int GetScore() const;
    void GetPosition(int& outX, int& outY) const;
    void DisplayStatus() const;
    void SetStatus(PlayerStatus newStatus);
    static std::string PlayerStatusToString(PlayerStatus status);
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
