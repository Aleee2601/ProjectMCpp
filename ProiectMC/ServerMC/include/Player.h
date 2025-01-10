#pragma once
#include <string>  // Pentru std::string
#include "Weapon.h"
#include "Map.h"

// Enum pentru statusul juc?torului
enum class PlayerStatus { ACTIVE, ELIMINATED };
enum class Direction { UP, DOWN, LEFT, RIGHT };

const int MAX_HITS = 3;          // Num?r maxim de lovituri pe care le poate primi un juc?tor
const int SCORE_FOR_UPGRADE = 500; // Scor necesar pentru a îmbun?t??i arma

class Player {
private:
    int m_id;                     // ID-ul unic al juc?torului
    int m_x, m_y;                 // Pozi?ia curent? a juc?torului
    int m_score;                  // Scorul curent al juc?torului
    std::string m_name;           // Numele juc?torului
    PlayerStatus m_status;        // Statusul curent al juc?torului (ACTIVE sau ELIMINATED)
    int m_hitsTaken;              // Num?rul de lovituri primite de juc?tor
    Weapon m_weapon;              // Arma juc?torului
    Direction m_direction;        // Direc?ia curent? a juc?torului

public:
    // Constructor: Ini?ializeaz? un juc?tor cu ID, nume, pozi?ie ini?ial? ?i direc?ie
    Player(int id, const std::string& name, int startX, int startY, Direction startDirection = Direction::UP)
        : m_id(id), m_name(name), m_x(startX), m_y(startY), m_score(0), m_status(PlayerStatus::ACTIVE),
        m_hitsTaken(0), m_weapon(), m_direction(startDirection) {}

    // Seteaz? pozi?ia juc?torului
    void SetPosition(int newX, int newY);

    // Adaug? puncte la scorul juc?torului
    void AddScore(int points);

    // Returneaz? scorul curent al juc?torului
    int GetScore() const;

    // Ob?ine pozi?ia curent? a juc?torului
    void GetPosition(int& outX, int& outY) const; // Linia 67 - Corect?

    // Afi?eaz? statusul curent al juc?torului
    void DisplayStatus() const;

    // Seteaz? statusul juc?torului (ACTIVE sau ELIMINATED)
    void SetStatus(PlayerStatus newStatus);

    // Returneaz? statusul curent al juc?torului
    PlayerStatus GetStatus() const;

    // Reseteaz? pozi?ia juc?torului la coordonatele ini?iale
    void ResetPosition(int startX, int startY);

    // Returneaz? numele juc?torului
    std::string GetName() const { return m_name; }

    // Returneaz? ID-ul juc?torului
    int GetId() const;

    // Returneaz? coordonata x a juc?torului
    int GetX() const;

    // Returneaz? coordonata y a juc?torului
    int GetY() const;

    // Mi?c? juc?torul într-o direc?ie specificat? ?i actualizeaz? harta
    void Move(Direction direction, const Map& gameMap);

    // Returneaz? direc?ia curent? a juc?torului
    Direction GetDirection() const { return m_direction; }

    // Înregistreaz? o lovitur? ?i actualizeaz? statusul, dac? este necesar
    void TakeHit();

    // Verific? dac? juc?torul este eliminat
    bool IsEliminated() const;

    // Adaug? puncte pentru lovirea unui alt juc?tor
    void AddScoreForHit();

    // Îmbun?t??e?te arma juc?torului
    void UpgradeWeapon();

    // Aplic? un power-up juc?torului
    void ApplyPowerUp();
};
