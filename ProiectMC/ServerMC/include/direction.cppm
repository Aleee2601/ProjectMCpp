// direction.cppm
export module direction; // Specifică că acesta este un modul exportat
import <iostream>;

export enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

export inline std::pair<int, int> GetNextPosition(int x, int y, Direction direction) {
    switch (direction) {
    case Direction::UP:    return { x-1, y };
    case Direction::DOWN:  return { x+1, y };
    case Direction::LEFT:  return { x, y-1 };
    case Direction::RIGHT: return { x, y+1 };
    default:               return { x, y };
    }
}
