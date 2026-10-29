#ifndef SNAKE_GAME_COLLISION_DETECTOR_H
#define SNAKE_GAME_COLLISION_DETECTOR_H

#include "grid_types.h"

namespace snakegame {

class CollisionDetector {
public:
    // Check the proposed head before advancing the snake. The controller supplies
    // a valid head-to-tail body and whether this move will grow it.
    // Throws std::invalid_argument for empty bodies or non-positive board sizes.
    [[nodiscard]] CollisionType check(Cell nextHead, const Body& body,
                                      BoardBounds bounds, bool grows) const;
};

} // namespace snakegame

#endif
