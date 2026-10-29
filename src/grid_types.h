#ifndef SNAKE_GAME_GRID_TYPES_H
#define SNAKE_GAME_GRID_TYPES_H

#include <vector>

namespace snakegame {

struct Cell {
    int x;
    int y;
};

constexpr bool operator==(Cell lhs, Cell rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

struct BoardBounds {
    int columns;
    int rows;
};

// Ordered from the current head to the current tail, in grid cells (not pixels).
using Body = std::vector<Cell>;

enum class CollisionType { None, Boundary, Self };

} // namespace snakegame

#endif
