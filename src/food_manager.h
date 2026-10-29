#ifndef SNAKE_GAME_FOOD_MANAGER_H
#define SNAKE_GAME_FOOD_MANAGER_H

#include "grid_types.h"
#include <optional>
#include <random>
#include <vector>

namespace snakegame {

enum class PlacementResult {
    Placed,
    BoardFull,
    InvalidBounds
};

class FoodManager {
public:
    FoodManager();

    // Check if food currently exists at the given cell.
    [[nodiscard]] bool isAt(Cell candidate) const;

    // Place food on an unoccupied cell within bounds using the provided random generator.
    PlacementResult place(BoardBounds bounds, const Body& body, std::mt19937& rng);

    // Place food using an internal random generator.
    PlacementResult place(BoardBounds bounds, const Body& body);

    // Get current food cell, if any.
    [[nodiscard]] std::optional<Cell> position() const;

    // Explicitly set food position (useful for deterministic tests).
    void setPosition(Cell cell);

    // Reset food state.
    void reset();

private:
    std::optional<Cell> foodCell;
    std::mt19937 defaultRng;
};

} // namespace snakegame

#endif // SNAKE_GAME_FOOD_MANAGER_H

