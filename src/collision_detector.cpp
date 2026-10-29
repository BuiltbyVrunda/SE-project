#include "collision_detector.h"

#include <cstddef>
#include <stdexcept>

namespace snakegame {

CollisionType CollisionDetector::check(Cell nextHead, const Body& body,
                                       BoardBounds bounds, bool grows) const {
    if (bounds.columns <= 0 || bounds.rows <= 0 || body.empty()) {
        throw std::invalid_argument("Collision checks require positive bounds and a non-empty body");
    }

    if (nextHead.x < 0 || nextHead.y < 0 ||
        nextHead.x >= bounds.columns || nextHead.y >= bounds.rows) {
        return CollisionType::Boundary;
    }

    // A non-growing move removes the tail in the same tick, freeing its cell.
    const std::size_t occupiedCount = body.size() - (grows ? 0 : 1);
    for (std::size_t i = 0; i < occupiedCount; ++i) {
        if (nextHead == body[i]) {
            return CollisionType::Self;
        }
    }
    return CollisionType::None;
}

} // namespace snakegame
