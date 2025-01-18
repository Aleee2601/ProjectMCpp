// bullet.cppm
export module bullet; // Specifică că acesta este un modul exportat
import direction;  // Importă modulul direction

export class Bullet {
private:
    int m_x, m_y;
    Direction m_direction;
    bool m_active;
    int m_ownerId;

public:
    Bullet(int startX, int startY, Direction dir, int ownerId)
        : m_x(startX), m_y(startY), m_direction(dir), m_active(true), m_ownerId(ownerId) {
    }

    bool IsInactive() const { return !m_active; }

    void SetInactive() { m_active = false; }
    void SetPosition(int nextX, int nextY) { m_x = nextX; m_y = nextY; }
    int GetX() const { return m_x; }
    int GetY() const { return m_y; }
    Direction GetDirection() const { return m_direction; }
    int GetOwnerId() const { return m_ownerId; }
};
