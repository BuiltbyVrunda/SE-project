#include "food_manager.h"
#include <algorithm>
#include <chrono>

namespace snakegame {

FoodManager::FoodManager()
    : foodCell(std::nullopt),
      defaultRng(static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()))
{
}

bool FoodManager::isAt(Cell candidate) const {
    return foodCell.has_value() && (*foodCell == candidate);
}

std::optional<Cell> FoodManager::position() const {
    return foodCell;
}

void FoodManager::setPosition(Cell cell) {
    foodCell = cell;
}

void FoodManager::reset() {
    foodCell = std::nullopt;
}

PlacementResult FoodManager::place(BoardBounds bounds, const Body& body, std::mt19937& rng) {
    if (bounds.columns <= 0 || bounds.rows <= 0) {
        foodCell = std::nullopt;
        return PlacementResult::InvalidBounds;
    }

    const int totalCells = bounds.columns * bounds.rows;
    if (static_cast<int>(body.size()) >= totalCells) {
        foodCell = std::nullopt;
        return PlacementResult::BoardFull;
    }

    // Identify occupied cells
    std::vector<bool> occupied(totalCells, false);
    for (const auto& segment : body) {
        if (segment.x >= 0 && segment.x < bounds.columns &&
            segment.y >= 0 && segment.y < bounds.rows) {
            occupied[segment.y * bounds.columns + segment.x] = true;
        }
    }

    // Collect available free cells
    std::vector<Cell> freeCells;
    freeCells.reserve(totalCells - body.size());
    for (int y = 0; y < bounds.rows; ++y) {
        for (int x = 0; x < bounds.columns; ++x) {
            if (!occupied[y * bounds.columns + x]) {
                freeCells.push_back(Cell{x, y});
            }
        }
    }

    if (freeCells.empty()) {
        foodCell = std::nullopt;
        return PlacementResult::BoardFull;
    }

    std::uniform_int_distribution<std::size_t> dist(0, freeCells.size() - 1);
    foodCell = freeCells[dist(rng)];
    return PlacementResult::Placed;
}

PlacementResult FoodManager::place(BoardBounds bounds, const Body& body) {
    return place(bounds, body, defaultRng);
}

} // namespace snakegame

