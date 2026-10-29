#include "collision_detector.h"

#include <gtest/gtest.h>
#include <limits>
#include <stdexcept>

namespace snakegame {
namespace {

const CollisionDetector detector;
const BoardBounds bounds{10, 8};
const Body body{{5, 4}, {4, 4}, {3, 4}};

class WallCollisionTest : public ::testing::TestWithParam<Cell> {};

TEST_P(WallCollisionTest, RejectsOutOfBoundsHeadWithAndWithoutGrowth) {
    for (bool grows : {false, true}) {
        EXPECT_EQ(detector.check(GetParam(), body, bounds, grows), CollisionType::Boundary);
    }
}

INSTANTIATE_TEST_SUITE_P(AllWalls, WallCollisionTest,
    ::testing::Values(Cell{-1, 4}, Cell{10, 4}, Cell{5, -1}, Cell{5, 8},
                      Cell{std::numeric_limits<int>::min(), 4},
                      Cell{5, std::numeric_limits<int>::max()}));

class ValidCellTest : public ::testing::TestWithParam<Cell> {};

TEST_P(ValidCellTest, AcceptsFreeCellsIncludingEdgesAndCorners) {
    for (bool grows : {false, true}) {
        EXPECT_EQ(detector.check(GetParam(), body, bounds, grows), CollisionType::None);
    }
}

INSTANTIATE_TEST_SUITE_P(InsideBoard, ValidCellTest,
    ::testing::Values(Cell{0, 0}, Cell{9, 0}, Cell{0, 7}, Cell{9, 7}, Cell{6, 4}));

TEST(CollisionDetectorTest, DetectsOccupiedNonTailCellWithAndWithoutGrowth) {
    EXPECT_EQ(detector.check({4, 4}, body, bounds, false), CollisionType::Self);
    EXPECT_EQ(detector.check({4, 4}, body, bounds, true), CollisionType::Self);
}

TEST(CollisionDetectorTest, MatchesTC05NonTailCollisionFixture) {
    // TC-05 layout A after its up and left moves; the next move is down.
    const Body beforeDown{{4, 4}, {5, 4}, {5, 5}, {4, 5}, {3, 5}, {2, 5}};
    EXPECT_EQ(detector.check({4, 5}, beforeDown, bounds, false), CollisionType::Self);
}

TEST(CollisionDetectorTest, AllowsTC05DepartingTailOnNonGrowingMove) {
    // TC-05 layout B after its up and left moves; no food is on the route.
    const Body beforeDown{{4, 4}, {5, 4}, {5, 5}, {4, 5}};
    EXPECT_EQ(detector.check({4, 5}, beforeDown, bounds, false), CollisionType::None);
}

TEST(CollisionDetectorTest, TailRemainsOccupiedOnGrowingMove) {
    const Body beforeDown{{4, 4}, {5, 4}, {5, 5}, {4, 5}};
    EXPECT_EQ(detector.check({4, 5}, beforeDown, bounds, true), CollisionType::Self);
}

TEST(CollisionDetectorTest, ChecksAllNonTailSegmentsOfLongSnake) {
    const Body longBody{{5, 5}, {4, 5}, {3, 5}, {2, 5}, {1, 5}, {0, 5}, {0, 4}, {1, 4}};
    for (std::size_t i = 0; i + 1 < longBody.size(); ++i) {
        EXPECT_EQ(detector.check(longBody[i], longBody, bounds, false), CollisionType::Self);
    }
    EXPECT_EQ(detector.check(longBody.back(), longBody, bounds, false), CollisionType::None);
}

TEST(CollisionDetectorTest, HandlesSingleSegmentBody) {
    const Body one{{0, 0}};
    EXPECT_EQ(detector.check({1, 0}, one, {2, 1}, false), CollisionType::None);
    EXPECT_EQ(detector.check({1, 0}, one, {2, 1}, true), CollisionType::None);
    EXPECT_EQ(detector.check({-1, 0}, one, {2, 1}, false), CollisionType::Boundary);
}

TEST(CollisionDetectorTest, DoesNotMutateBodyOnAnyOutcome) {
    Body input = body;
    const Body original = input;
    EXPECT_EQ(detector.check({6, 4}, input, bounds, false), CollisionType::None);
    EXPECT_EQ(input, original);
    EXPECT_EQ(detector.check({4, 4}, input, bounds, false), CollisionType::Self);
    EXPECT_EQ(input, original);
    EXPECT_EQ(detector.check({10, 4}, input, bounds, true), CollisionType::Boundary);
    EXPECT_EQ(input, original);
}

TEST(CollisionDetectorTest, RejectsEmptyBodyWithoutUnderflow) {
    EXPECT_THROW((void)detector.check({0, 0}, {}, bounds, false), std::invalid_argument);
    EXPECT_THROW((void)detector.check({0, 0}, {}, bounds, true), std::invalid_argument);
}

class InvalidBoundsTest : public ::testing::TestWithParam<BoardBounds> {};

TEST_P(InvalidBoundsTest, RejectsInvalidConfiguration) {
    EXPECT_THROW((void)detector.check({6, 4}, body, GetParam(), false), std::invalid_argument);
}

INSTANTIATE_TEST_SUITE_P(NonPositiveDimensions, InvalidBoundsTest,
    ::testing::Values(BoardBounds{0, 8}, BoardBounds{10, 0},
                      BoardBounds{-1, 8}, BoardBounds{10, -1}));

} // namespace
} // namespace snakegame
