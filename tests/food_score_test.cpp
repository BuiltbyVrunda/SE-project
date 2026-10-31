// Unit tests for the food and scoring module (Vrunda C).
// Covers Test Plan TC-03 (food placement and detection) and TC-04 (score update),
// requirements FR-07 to FR-11. Pure logic: no SFML window is needed.

#include <gtest/gtest.h>

#include <algorithm>
#include <random>
#include <set>
#include <utility>

#include "food_manager.h"
#include "score_manager.h"

namespace snakegame {
namespace {

bool insideBounds(Cell cell, BoardBounds bounds)
{
    return cell.x >= 0 && cell.x < bounds.columns &&
           cell.y >= 0 && cell.y < bounds.rows;
}

bool onBody(Cell cell, const Body& body)
{
    return std::find(body.begin(), body.end(), cell) != body.end();
}

// ---------------------------------------------------------------------------
// FoodManager: placement (TC-03, FR-07)
// ---------------------------------------------------------------------------

TEST(FoodPlacementTest, NoFoodExistsBeforeFirstPlacement)
{
    FoodManager food;
    EXPECT_FALSE(food.position().has_value());
    EXPECT_FALSE(food.isAt(Cell{0, 0}));
}

TEST(FoodPlacementTest, FoodIsAlwaysInsideBoardAndOffSnakeForManySeeds)
{
    const BoardBounds bounds{10, 8};
    const Body body{{5, 5}, {4, 5}, {3, 5}, {3, 4}, {3, 3}, {4, 3}};

    for (unsigned int seed = 0; seed < 500; ++seed)
    {
        FoodManager food;
        std::mt19937 rng(seed);
        ASSERT_EQ(food.place(bounds, body, rng), PlacementResult::Placed);
        ASSERT_TRUE(food.position().has_value());
        const Cell placed = *food.position();
        EXPECT_TRUE(insideBounds(placed, bounds)) << "seed " << seed;
        EXPECT_FALSE(onBody(placed, body)) << "seed " << seed;
    }
}

TEST(FoodPlacementTest, SameSeedGivesSamePositionForRepeatableTests)
{
    const BoardBounds bounds{40, 30};
    const Body body{{5, 5}, {4, 5}, {3, 5}};

    FoodManager first;
    FoodManager second;
    std::mt19937 rngA(1234);
    std::mt19937 rngB(1234);
    first.place(bounds, body, rngA);
    second.place(bounds, body, rngB);

    ASSERT_TRUE(first.position().has_value());
    ASSERT_TRUE(second.position().has_value());
    EXPECT_EQ(*first.position(), *second.position());
}

TEST(FoodPlacementTest, OnlyRemainingFreeCellIsChosen)
{
    // 3x3 board with every cell except (2, 2) occupied by the snake.
    const BoardBounds bounds{3, 3};
    const Body body{{0, 0}, {1, 0}, {2, 0}, {2, 1}, {1, 1}, {0, 1}, {0, 2}, {1, 2}};

    for (unsigned int seed = 0; seed < 50; ++seed)
    {
        FoodManager food;
        std::mt19937 rng(seed);
        ASSERT_EQ(food.place(bounds, body, rng), PlacementResult::Placed);
        EXPECT_EQ(*food.position(), (Cell{2, 2}));
    }
}

TEST(FoodPlacementTest, EveryFreeCellCanBeChosen)
{
    // The choice must be random over all free cells, not stuck on one of them.
    const BoardBounds bounds{3, 3};
    const Body body{{0, 0}, {1, 0}, {2, 0}, {1, 1}};
    const std::set<std::pair<int, int>> expectedFree{{2, 1}, {0, 1}, {0, 2}, {1, 2}, {2, 2}};

    std::set<std::pair<int, int>> seen;
    FoodManager food;
    std::mt19937 rng(42);
    for (int run = 0; run < 1000; ++run)
    {
        ASSERT_EQ(food.place(bounds, body, rng), PlacementResult::Placed);
        seen.insert({food.position()->x, food.position()->y});
    }
    EXPECT_EQ(seen, expectedFree);
}

TEST(FoodPlacementTest, BoardFullClearsFoodAndReportsIt)
{
    FoodManager food;
    food.setPosition(Cell{1, 1});

    const BoardBounds bounds{2, 2};
    const Body body{{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    EXPECT_EQ(food.place(bounds, body), PlacementResult::BoardFull);
    EXPECT_FALSE(food.position().has_value());
}

TEST(FoodPlacementTest, InvalidBoardClearsFoodAndReportsIt)
{
    FoodManager food;
    food.setPosition(Cell{1, 1});

    const Body body{{0, 0}};
    EXPECT_EQ(food.place(BoardBounds{0, 10}, body), PlacementResult::InvalidBounds);
    EXPECT_FALSE(food.position().has_value());
    EXPECT_EQ(food.place(BoardBounds{10, -3}, body), PlacementResult::InvalidBounds);
    EXPECT_FALSE(food.position().has_value());
}

TEST(FoodPlacementTest, NewPlacementReplacesOldFood)
{
    // Only one food item exists at a time (FR-11).
    const BoardBounds bounds{3, 1};
    FoodManager food;
    food.setPosition(Cell{0, 0});

    // (0,0) is now occupied by the snake, so the food must move off it.
    const Body body{{0, 0}, {1, 0}};
    ASSERT_EQ(food.place(bounds, body), PlacementResult::Placed);
    EXPECT_EQ(*food.position(), (Cell{2, 0}));
    EXPECT_FALSE(food.isAt(Cell{0, 0}));
}

// ---------------------------------------------------------------------------
// FoodManager: detection (TC-03, FR-08)
// ---------------------------------------------------------------------------

TEST(FoodDetectionTest, IsAtMatchesOnlyTheFoodCell)
{
    FoodManager food;
    food.setPosition(Cell{7, 3});

    EXPECT_TRUE(food.isAt(Cell{7, 3}));
    EXPECT_FALSE(food.isAt(Cell{3, 7}));
    EXPECT_FALSE(food.isAt(Cell{7, 4}));
    EXPECT_FALSE(food.isAt(Cell{6, 3}));
}

TEST(FoodDetectionTest, ResetRemovesFood)
{
    FoodManager food;
    food.setPosition(Cell{2, 2});
    food.reset();

    EXPECT_FALSE(food.position().has_value());
    EXPECT_FALSE(food.isAt(Cell{2, 2}));
}

// ---------------------------------------------------------------------------
// ScoreManager (TC-04, FR-10)
// ---------------------------------------------------------------------------

TEST(ScoreManagerTest, StartsAtZero)
{
    ScoreManager score;
    EXPECT_EQ(score.getScore(), 0);
    EXPECT_EQ(score.getBestScore(), 0);
    EXPECT_EQ(score.getFoodsEaten(), 0);
    EXPECT_EQ(score.getPointsPerFood(), 10);
}

TEST(ScoreManagerTest, EachFoodAddsExactlyTenPoints)
{
    ScoreManager score;
    score.addFoodPoints();
    EXPECT_EQ(score.getScore(), 10);
    EXPECT_EQ(score.getFoodsEaten(), 1);

    score.addFoodPoints();
    EXPECT_EQ(score.getScore(), 20);
    EXPECT_EQ(score.getFoodsEaten(), 2);
}

TEST(ScoreManagerTest, ScoreEqualsFoodCountTimesFoodValue)
{
    // TC-04 step 5: total = food count x food value after five foods.
    ScoreManager score;
    for (int i = 0; i < 5; ++i)
    {
        score.addFoodPoints();
    }
    EXPECT_EQ(score.getFoodsEaten(), 5);
    EXPECT_EQ(score.getScore(), score.getFoodsEaten() * score.getPointsPerFood());
    EXPECT_EQ(score.getScore(), 50);
}

TEST(ScoreManagerTest, BestScoreFollowsHighestScore)
{
    ScoreManager score;
    score.addFoodPoints();
    score.addFoodPoints();
    score.addFoodPoints();
    EXPECT_EQ(score.getBestScore(), 30);
}

TEST(ScoreManagerTest, ResetRoundClearsScoreButKeepsBest)
{
    ScoreManager score;
    score.addFoodPoints();
    score.addFoodPoints();
    score.resetRound();

    EXPECT_EQ(score.getScore(), 0);
    EXPECT_EQ(score.getFoodsEaten(), 0);
    EXPECT_EQ(score.getBestScore(), 20);
}

TEST(ScoreManagerTest, LowerLaterRoundDoesNotLowerBest)
{
    ScoreManager score;
    for (int i = 0; i < 4; ++i)
    {
        score.addFoodPoints();
    }
    score.resetRound();
    score.addFoodPoints();

    EXPECT_EQ(score.getScore(), 10);
    EXPECT_EQ(score.getBestScore(), 40);
}

TEST(ScoreManagerTest, HigherLaterRoundRaisesBest)
{
    ScoreManager score;
    score.addFoodPoints();
    score.resetRound();
    for (int i = 0; i < 3; ++i)
    {
        score.addFoodPoints();
    }
    EXPECT_EQ(score.getBestScore(), 30);
}

TEST(ScoreManagerTest, CustomFoodValueIsUsed)
{
    ScoreManager score(25);
    score.addFoodPoints();
    score.addFoodPoints();
    EXPECT_EQ(score.getPointsPerFood(), 25);
    EXPECT_EQ(score.getScore(), 50);
}

TEST(ScoreManagerTest, NonPositiveFoodValueFallsBackToDefault)
{
    ScoreManager zero(0);
    ScoreManager negative(-5);
    EXPECT_EQ(zero.getPointsPerFood(), ScoreManager::DefaultPointsPerFood);
    EXPECT_EQ(negative.getPointsPerFood(), ScoreManager::DefaultPointsPerFood);
}

} // namespace
} // namespace snakegame
