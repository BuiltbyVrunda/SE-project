// Integration tests for food, growth and scoring through the GameController (Vrunda C).
// Covers Test Plan TC-03 (food detection and snake growth) and TC-04 (score update
// and replacement food), requirements FR-07 to FR-11.
//
// Starting layout (from Snake::reset): head (5,5), body (4,5), tail (3,5), moving right.

#include <gtest/gtest.h>

#include <algorithm>

#include "game_controller.h"

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

TEST(FoodScoreIntegrationTest, InitialFoodIsInsideBoardAndOffSnake)
{
    // TC-01 step 6: one food item is visible inside the boundary and does not overlap the snake.
    for (int run = 0; run < 50; ++run)
    {
        GameController controller({40, 30});
        const auto food = controller.getFoodManager().position();
        ASSERT_TRUE(food.has_value());
        EXPECT_TRUE(insideBounds(*food, controller.getBounds()));
        EXPECT_FALSE(onBody(*food, controller.getSnake().getGridBody()));
        EXPECT_EQ(controller.getScore(), 0);
    }
}

TEST(FoodScoreIntegrationTest, EatingFoodGrowsSnakeByOneAndKeepsTail)
{
    GameController controller({40, 30});
    controller.startRound();
    controller.getFoodManager().setPosition(Cell{6, 5});

    ASSERT_TRUE(controller.step());

    const Body expected{{6, 5}, {5, 5}, {4, 5}, {3, 5}};
    EXPECT_EQ(controller.getSnake().getGridBody(), expected);
}

TEST(FoodScoreIntegrationTest, FoodNextToPathIsNotEaten)
{
    GameController controller({40, 30});
    controller.startRound();
    controller.getFoodManager().setPosition(Cell{6, 6}); // diagonal to the next head cell

    ASSERT_TRUE(controller.step());

    EXPECT_EQ(controller.getSnake().getGridBody().size(), 3u);
    EXPECT_EQ(controller.getScore(), 0);
    EXPECT_TRUE(controller.getFoodManager().isAt(Cell{6, 6}));
}

TEST(FoodScoreIntegrationTest, OneConsumptionEventPerFood)
{
    // TC-03 step 3: eating is registered once, not repeated on later moves.
    GameController controller({40, 30});
    controller.startRound();
    controller.getFoodManager().setPosition(Cell{6, 5});
    ASSERT_TRUE(controller.step());
    ASSERT_EQ(controller.getScore(), 10);

    controller.getFoodManager().setPosition(Cell{30, 20}); // keep food off the route
    for (int i = 0; i < 3; ++i)
    {
        ASSERT_TRUE(controller.step());
    }

    EXPECT_EQ(controller.getScore(), 10);
    EXPECT_EQ(controller.getFoodsEaten(), 1);
    EXPECT_EQ(controller.getSnake().getGridBody().size(), 4u);
}

TEST(FoodScoreIntegrationTest, FiveConsecutiveFoodsGiveFiftyPoints)
{
    // TC-04 step 5: five foods in a row, score = count x value, one food on screen each time.
    GameController controller({40, 30});
    controller.startRound();

    for (int eaten = 1; eaten <= 5; ++eaten)
    {
        const Cell nextHead = controller.getSnake().getNextHeadGrid();
        controller.getFoodManager().setPosition(nextHead);
        ASSERT_TRUE(controller.step());

        EXPECT_EQ(controller.getScore(), eaten * 10);
        EXPECT_EQ(controller.getFoodsEaten(), eaten);
        EXPECT_EQ(controller.getSnake().getGridBody().size(), static_cast<std::size_t>(3 + eaten));

        const auto replacement = controller.getFoodManager().position();
        ASSERT_TRUE(replacement.has_value());
        EXPECT_TRUE(insideBounds(*replacement, controller.getBounds()));
        EXPECT_FALSE(onBody(*replacement, controller.getSnake().getGridBody()));
    }
    EXPECT_EQ(controller.getState(), GameState::Playing);
}

TEST(FoodScoreIntegrationTest, ReplacementFoodNeverLandsOnLongSnake)
{
    // Grow the snake across most of a row and check every replacement food.
    GameController controller({40, 30});
    controller.startRound();

    for (int eaten = 1; eaten <= 30; ++eaten)
    {
        controller.getFoodManager().setPosition(controller.getSnake().getNextHeadGrid());
        ASSERT_TRUE(controller.step()) << "food " << eaten;

        const auto replacement = controller.getFoodManager().position();
        ASSERT_TRUE(replacement.has_value());
        EXPECT_FALSE(onBody(*replacement, controller.getSnake().getGridBody())) << "food " << eaten;
        EXPECT_TRUE(insideBounds(*replacement, controller.getBounds())) << "food " << eaten;
    }
    EXPECT_EQ(controller.getScore(), 300);
    EXPECT_EQ(controller.getSnake().getGridBody().size(), 33u);
}

TEST(FoodScoreIntegrationTest, FinalScoreIsKeptAtGameOver)
{
    // TC-05 step 8: final score equals the score at the moment of collision.
    GameController controller({10, 10});
    controller.startRound();
    controller.getFoodManager().setPosition(Cell{6, 5});
    ASSERT_TRUE(controller.step()); // score 10, head (6,5)

    controller.getFoodManager().setPosition(Cell{0, 0});
    while (controller.step())
    {
    }

    EXPECT_EQ(controller.getState(), GameState::GameOver);
    EXPECT_EQ(controller.getScore(), 10);
    EXPECT_EQ(controller.getBestScore(), 10);
}

TEST(FoodScoreIntegrationTest, RestartResetsScoreButKeepsBestScore)
{
    GameController controller({10, 10});
    controller.startRound();

    controller.getFoodManager().setPosition(Cell{6, 5});
    ASSERT_TRUE(controller.step());
    controller.getFoodManager().setPosition(Cell{7, 5});
    ASSERT_TRUE(controller.step());
    ASSERT_EQ(controller.getScore(), 20);

    controller.getFoodManager().setPosition(Cell{0, 0});
    while (controller.step())
    {
    }
    ASSERT_EQ(controller.getState(), GameState::GameOver);

    controller.restart();
    EXPECT_EQ(controller.getScore(), 0);
    EXPECT_EQ(controller.getFoodsEaten(), 0);
    EXPECT_EQ(controller.getBestScore(), 20);

    // A lower score in the next round does not lower the best score.
    controller.getFoodManager().setPosition(Cell{6, 5});
    ASSERT_TRUE(controller.step());
    EXPECT_EQ(controller.getScore(), 10);
    EXPECT_EQ(controller.getBestScore(), 20);
}

TEST(FoodScoreIntegrationTest, RestartPlacesNewFoodOffResetSnake)
{
    // TC-06 step 6: new food is placed at a valid position after restart.
    for (int run = 0; run < 20; ++run)
    {
        GameController controller({40, 30});
        controller.startRound();
        controller.getFoodManager().setPosition(Cell{6, 5});
        ASSERT_TRUE(controller.step());

        controller.restart();
        const auto food = controller.getFoodManager().position();
        ASSERT_TRUE(food.has_value());
        EXPECT_TRUE(insideBounds(*food, controller.getBounds()));
        EXPECT_FALSE(onBody(*food, controller.getSnake().getGridBody()));
    }
}

TEST(FoodScoreIntegrationTest, NoScoreChangeAfterGameOver)
{
    GameController controller({6, 6});
    controller.startRound();
    controller.getFoodManager().setPosition(Cell{0, 0});
    ASSERT_FALSE(controller.step()); // (6,5) is outside a 6x6 board
    ASSERT_EQ(controller.getState(), GameState::GameOver);

    controller.getFoodManager().setPosition(Cell{5, 4});
    controller.update(2.0f);
    EXPECT_FALSE(controller.step());
    EXPECT_EQ(controller.getScore(), 0);
    EXPECT_EQ(controller.getFoodsEaten(), 0);
}

} // namespace
} // namespace snakegame
