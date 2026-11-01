#include <gtest/gtest.h>
#include "game_controller.h"
#include "food_manager.h"
#include "collision_detector.h"

namespace snakegame {
namespace {

// Scenario 1: Normal snake movement
TEST(GameIntegrationTest, Scenario01_NormalSnakeMovement) {
    GameController controller({40, 30});
    controller.startRound();
    EXPECT_EQ(controller.getState(), GameState::Playing);

    const Cell initialHead = controller.getSnake().getGridBody().front();
    EXPECT_EQ(initialHead, (Cell{5, 5})); // 100 / 20 = 5

    // Prevent immediate food collision by placing food far away
    controller.getFoodManager().setPosition(Cell{20, 20});

    const bool ok = controller.step();
    EXPECT_TRUE(ok);

    const Cell newHead = controller.getSnake().getGridBody().front();
    EXPECT_EQ(newHead, (Cell{6, 5})); // Moved 1 cell right
    EXPECT_EQ(controller.getSnake().getGridBody().size(), 3u); // Length unchanged
}

// Scenario 2: Movement in permitted directions
TEST(GameIntegrationTest, Scenario02_MovementInPermittedDirections) {
    GameController controller({40, 30});
    controller.startRound();
    controller.getFoodManager().setPosition(Cell{30, 25});

    // Currently moving Right. Turn Up:
    EXPECT_TRUE(controller.handleDirectionInput({0.f, -20.f}));
    EXPECT_TRUE(controller.step());
    EXPECT_EQ(controller.getSnake().getGridBody().front(), (Cell{5, 4}));

    // Turn Left:
    EXPECT_TRUE(controller.handleDirectionInput({-20.f, 0.f}));
    EXPECT_TRUE(controller.step());
    EXPECT_EQ(controller.getSnake().getGridBody().front(), (Cell{4, 4}));

    // Turn Down:
    EXPECT_TRUE(controller.handleDirectionInput({0.f, 20.f}));
    EXPECT_TRUE(controller.step());
    EXPECT_EQ(controller.getSnake().getGridBody().front(), (Cell{4, 5}));

    // Turn Right:
    EXPECT_TRUE(controller.handleDirectionInput({20.f, 0.f}));
    EXPECT_TRUE(controller.step());
    EXPECT_EQ(controller.getSnake().getGridBody().front(), (Cell{5, 5}));
}

// Scenario 3: Prevention of invalid reverse-direction movement
TEST(GameIntegrationTest, Scenario03_PreventInvalidReverseDirection) {
    GameController controller({40, 30});
    controller.startRound();

    // Snake starts moving Right. Attempt direct reversal to Left:
    EXPECT_FALSE(controller.handleDirectionInput({-20.f, 0.f}));
    EXPECT_EQ(controller.getSnake().getDirection(), sf::Vector2f(20.f, 0.f));

    // Turn Up (valid):
    EXPECT_TRUE(controller.handleDirectionInput({0.f, -20.f}));
    // Rapid second turn before step must be rejected (input latch):
    EXPECT_FALSE(controller.handleDirectionInput({-20.f, 0.f}));
    EXPECT_EQ(controller.getSnake().getDirection(), sf::Vector2f(0.f, -20.f));
}

// Scenario 4: Wall collision
TEST(GameIntegrationTest, Scenario04_WallCollisionDetection) {
    // 10x10 board. Snake head at (5, 5) moving right.
    // Cells: 5 -> 6 -> 7 -> 8 -> 9 (last in-bounds) -> 10 (wall collision)
    GameController controller({10, 10});
    controller.startRound();
    controller.getFoodManager().setPosition(Cell{0, 0});

    // Step to 6, 7, 8, 9
    for (int x = 6; x <= 9; ++x) {
        EXPECT_TRUE(controller.step());
        EXPECT_EQ(controller.getState(), GameState::Playing);
        EXPECT_EQ(controller.getSnake().getGridBody().front().x, x);
    }

    // Now head is at x=9. Next step would hit x=10 (out of bounds).
    const Body bodyBefore = controller.getSnake().getGridBody();
    const bool stepResult = controller.step();

    EXPECT_FALSE(stepResult);
    EXPECT_EQ(controller.getState(), GameState::GameOver);
    EXPECT_EQ(controller.getLastCollision(), CollisionType::Boundary);
    // Crucial check: snake position must NOT have advanced past the boundary!
    EXPECT_EQ(controller.getSnake().getGridBody(), bodyBefore);
}

// Scenario 5: Self-collision
TEST(GameIntegrationTest, Scenario05_SelfCollisionDetection) {
    GameController controller({20, 20});
    controller.startRound();

    // Grow the snake so it has length >= 5 to allow self-collision
    // Snake initial: (5,5), (4,5), (3,5)
    // Place food sequentially in path
    controller.getFoodManager().setPosition(Cell{6, 5});
    controller.step(); // Eats at (6, 5), length becomes 4
    EXPECT_EQ(controller.getSnake().getGridBody().size(), 4u);

    controller.getFoodManager().setPosition(Cell{7, 5});
    controller.step(); // Eats at (7, 5), length becomes 5
    EXPECT_EQ(controller.getSnake().getGridBody().size(), 5u);

    // Snake body now: head at (7, 5), (6, 5), (5, 5), (4, 5), (3, 5)
    // Move Up: head -> (7, 4)
    controller.handleDirectionInput({0.f, -20.f});
    controller.step();

    // Move Left: head -> (6, 4)
    controller.handleDirectionInput({-20.f, 0.f});
    controller.step();

    // Move Down: next head is (6, 5), which is occupied by the snake's body!
    controller.handleDirectionInput({0.f, 20.f});
    const Body bodyBefore = controller.getSnake().getGridBody();
    const bool stepResult = controller.step();

    EXPECT_FALSE(stepResult);
    EXPECT_EQ(controller.getState(), GameState::GameOver);
    EXPECT_EQ(controller.getLastCollision(), CollisionType::Self);
    EXPECT_EQ(controller.getSnake().getGridBody(), bodyBefore);
}

// Scenario 6: Food consumption and score increment
TEST(GameIntegrationTest, Scenario06_FoodConsumptionAndScoreIncrement) {
    GameController controller({30, 30});
    controller.startRound();
    EXPECT_EQ(controller.getScore(), 0);
    EXPECT_EQ(controller.getSnake().getGridBody().size(), 3u);

    // Place food directly at next head position (6, 5)
    controller.getFoodManager().setPosition(Cell{6, 5});

    const bool ok = controller.step();
    EXPECT_TRUE(ok);
    EXPECT_EQ(controller.getState(), GameState::Playing);
    EXPECT_EQ(controller.getScore(), 10);
    EXPECT_EQ(controller.getSnake().getGridBody().size(), 4u);
    EXPECT_EQ(controller.getSnake().getGridBody().front(), (Cell{6, 5}));

    // Food position must have been replaced with a valid free cell
    const auto newFood = controller.getFoodManager().position();
    EXPECT_TRUE(newFood.has_value());
    EXPECT_NE(*newFood, (Cell{6, 5})); // Replacement food placed
}

// Scenario 7: Game Over behavior
TEST(GameIntegrationTest, Scenario07_GameOverBehavior) {
    GameController controller({6, 6});
    controller.startRound();
    controller.getFoodManager().setPosition(Cell{0, 0});

    // Head is at (5, 5) on a 6x6 board (0..5). One step right hits boundary (6, 5).
    EXPECT_FALSE(controller.step());
    EXPECT_EQ(controller.getState(), GameState::GameOver);
    EXPECT_EQ(controller.getLastCollision(), CollisionType::Boundary);
}

// Scenario 8: Prevention of movement after Game Over
TEST(GameIntegrationTest, Scenario08_PreventionOfMovementAfterGameOver) {
    GameController controller({6, 6});
    controller.startRound();
    controller.getFoodManager().setPosition(Cell{0, 0});

    // Hit wall
    controller.step();
    EXPECT_EQ(controller.getState(), GameState::GameOver);

    const Body bodyAfterGameOver = controller.getSnake().getGridBody();
    const int scoreAfterGameOver = controller.getScore();

    // Attempting further steps must return false and NOT advance the snake
    EXPECT_FALSE(controller.step());
    EXPECT_FALSE(controller.step());
    controller.update(1.0f); // Even after 1 second of simulation time

    EXPECT_EQ(controller.getState(), GameState::GameOver);
    EXPECT_EQ(controller.getSnake().getGridBody(), bodyAfterGameOver);
    EXPECT_EQ(controller.getScore(), scoreAfterGameOver);
}

// Scenario 9: Restart after Game Over
TEST(GameIntegrationTest, Scenario09_RestartAfterGameOver) {
    GameController controller({6, 6});
    controller.startRound();
    controller.step(); // Fatal collision
    EXPECT_EQ(controller.getState(), GameState::GameOver);

    // Call restart
    controller.restart();
    EXPECT_EQ(controller.getState(), GameState::Playing);
    EXPECT_EQ(controller.getLastCollision(), CollisionType::None);
}

// Scenario 10: Reset of position, direction, food, and score
TEST(GameIntegrationTest, Scenario10_ResetOfAllGameplayState) {
    GameController controller({40, 30});
    controller.startRound();

    // Eat one food to score 10 and grow
    controller.getFoodManager().setPosition(Cell{6, 5});
    controller.step();
    EXPECT_EQ(controller.getScore(), 10);
    EXPECT_EQ(controller.getSnake().getGridBody().size(), 4u);

    // Turn up
    controller.handleDirectionInput({0.f, -20.f});
    controller.step();
    EXPECT_NE(controller.getSnake().getDirection(), sf::Vector2f(20.f, 0.f));

    // Force restart
    controller.restart();

    // Verify completely clean state:
    EXPECT_EQ(controller.getScore(), 0);
    EXPECT_EQ(controller.getSnake().getGridBody().size(), 3u);
    EXPECT_EQ(controller.getSnake().getGridBody().front(), (Cell{5, 5}));
    EXPECT_EQ(controller.getSnake().getDirection(), sf::Vector2f(20.f, 0.f));
    EXPECT_EQ(controller.getLastCollision(), CollisionType::None);
    EXPECT_EQ(controller.getState(), GameState::Playing);

    const auto food = controller.getFoodManager().position();
    EXPECT_TRUE(food.has_value());
    // Food must not be placed on any segment of the reset snake
    for (const auto& seg : controller.getSnake().getGridBody()) {
        EXPECT_NE(*food, seg);
    }
}

// Scenario 11: Continued gameplay after restarting
TEST(GameIntegrationTest, Scenario11_ContinuedGameplayAfterRestart) {
    GameController controller({40, 30});
    controller.startRound();

    // Score points and then collide
    controller.getFoodManager().setPosition(Cell{6, 5});
    controller.step(); // Score = 10
    EXPECT_EQ(controller.getScore(), 10);

    // Restart
    controller.restart();
    EXPECT_EQ(controller.getScore(), 0);

    // Now play again: place food at (6, 5) again and advance
    controller.getFoodManager().setPosition(Cell{6, 5});
    EXPECT_TRUE(controller.step());
    EXPECT_EQ(controller.getScore(), 10);
    EXPECT_EQ(controller.getSnake().getGridBody().size(), 4u);

    // Continue moving
    controller.getFoodManager().setPosition(Cell{30, 20});
    EXPECT_TRUE(controller.step());
    EXPECT_EQ(controller.getSnake().getGridBody().front(), (Cell{7, 5}));
    EXPECT_EQ(controller.getState(), GameState::Playing);
}

// Scenario 12: Enter the tail's actual current cell on a non-growing move.
TEST(GameIntegrationTest, Scenario12_TailDepartureRuleIntegration) {
    GameController controller({20, 20});
    controller.startRound();
    controller.getFoodManager().setPosition(Cell{6, 5});
    ASSERT_TRUE(controller.step()); // Length four, head at (6,5).
    controller.getFoodManager().setPosition(Cell{0, 0});

    ASSERT_TRUE(controller.handleDirectionInput({0.f, -20.f}));
    ASSERT_TRUE(controller.step()); // (6,4)
    ASSERT_TRUE(controller.handleDirectionInput({-20.f, 0.f}));
    ASSERT_TRUE(controller.step()); // (5,4)
    ASSERT_TRUE(controller.handleDirectionInput({0.f, 20.f}));

    const Cell departingTail = controller.getSnake().getGridBody().back();
    ASSERT_EQ(departingTail, (Cell{5, 5}));
    ASSERT_EQ(controller.getSnake().getNextHeadGrid(), departingTail);
    ASSERT_FALSE(controller.getFoodManager().isAt(departingTail));
    EXPECT_TRUE(controller.step());
    EXPECT_EQ(controller.getState(), GameState::Playing);
    EXPECT_EQ(controller.getSnake().getGridBody().front(), departingTail);
    EXPECT_EQ(controller.getSnake().getGridBody().size(), 4u);
}

class WallIntegrationTest : public ::testing::TestWithParam<Cell> {};

TEST_P(WallIntegrationTest, StopsBeforeEachWallAndIgnoresFurtherMovement) {
    GameController controller({10, 10});
    controller.startRound();
    controller.getFoodManager().setPosition(Cell{0, 0});
    const Cell direction = GetParam();
    // Turn up once before heading left, avoiding an immediate reverse.
    if (direction.x == -1) {
        ASSERT_TRUE(controller.handleDirectionInput({0.f, -20.f}));
        ASSERT_TRUE(controller.step());
    }
    ASSERT_TRUE(controller.handleDirectionInput(
        {direction.x * 20.f, direction.y * 20.f}));

    Body bodyBefore;
    int moves = 0;
    while (controller.getState() == GameState::Playing && moves++ < 20) {
        bodyBefore = controller.getSnake().getGridBody();
        controller.step();
    }
    ASSERT_EQ(controller.getState(), GameState::GameOver);
    EXPECT_EQ(controller.getLastCollision(), CollisionType::Boundary);
    EXPECT_EQ(controller.getSnake().getGridBody(), bodyBefore);
    EXPECT_FALSE(controller.step());
    EXPECT_FALSE(controller.handleDirectionInput({0.f, -20.f}));
    controller.update(1.0f);
    EXPECT_EQ(controller.getSnake().getGridBody(), bodyBefore);
}

INSTANTIATE_TEST_SUITE_P(AllFourWalls, WallIntegrationTest,
    ::testing::Values(Cell{1, 0}, Cell{-1, 0}, Cell{0, -1}, Cell{0, 1}));

TEST(GameIntegrationTest, FatalCollisionKeepsEarnedScoreAndRestartClearsIt) {
    GameController controller({10, 10});
    controller.startRound();
    controller.getFoodManager().setPosition(Cell{6, 5});
    ASSERT_TRUE(controller.step());
    ASSERT_EQ(controller.getScore(), 10);
    controller.getFoodManager().setPosition(Cell{0, 0});
    for (int x = 7; x <= 9; ++x) {
        ASSERT_TRUE(controller.step());
    }
    const Body lastValidBody = controller.getSnake().getGridBody();
    ASSERT_FALSE(controller.step());
    ASSERT_EQ(controller.getState(), GameState::GameOver);
    EXPECT_EQ(controller.getScore(), 10);
    EXPECT_EQ(controller.getSnake().getGridBody(), lastValidBody);
    controller.update(2.f);
    EXPECT_EQ(controller.getScore(), 10);
    EXPECT_EQ(controller.getSnake().getGridBody(), lastValidBody);

    controller.restart();
    EXPECT_EQ(controller.getState(), GameState::Playing);
    EXPECT_EQ(controller.getScore(), 0);
    EXPECT_EQ(controller.getLastCollision(), CollisionType::None);
    EXPECT_EQ(controller.getSnake().getGridBody(), (Body{{5, 5}, {4, 5}, {3, 5}}));
    EXPECT_EQ(controller.getSnake().getDirection(), sf::Vector2f(20.f, 0.f));
    ASSERT_TRUE(controller.getFoodManager().position().has_value());
    const Cell food = *controller.getFoodManager().position();
    EXPECT_GE(food.x, 0);
    EXPECT_LT(food.x, 10);
    EXPECT_GE(food.y, 0);
    EXPECT_LT(food.y, 10);
    for (Cell segment : controller.getSnake().getGridBody()) {
        EXPECT_NE(food, segment);
    }
    controller.getFoodManager().setPosition(Cell{0, 0});
    EXPECT_TRUE(controller.step());
    EXPECT_EQ(controller.getSnake().getGridBody().front(), (Cell{6, 5}));
}

// FoodManager unit tests
TEST(FoodManagerTest, PlacementAvoidsAllSnakeSegments) {
    FoodManager foodMgr;
    BoardBounds bounds{5, 5};
    Body body{{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}};

    for (int run = 0; run < 20; ++run) {
        PlacementResult res = foodMgr.place(bounds, body);
        EXPECT_EQ(res, PlacementResult::Placed);
        EXPECT_TRUE(foodMgr.position().has_value());
        for (const auto& seg : body) {
            EXPECT_NE(*foodMgr.position(), seg);
        }
    }
}

TEST(FoodManagerTest, DetectsBoardFullGracefully) {
    FoodManager foodMgr;
    BoardBounds bounds{2, 2}; // 4 cells total
    Body body{{0, 0}, {1, 0}, {0, 1}, {1, 1}}; // All 4 occupied

    PlacementResult res = foodMgr.place(bounds, body);
    EXPECT_EQ(res, PlacementResult::BoardFull);
    EXPECT_FALSE(foodMgr.position().has_value());
}

TEST(FoodManagerTest, RejectsInvalidBounds) {
    FoodManager foodMgr;
    Body body{{0, 0}};
    EXPECT_EQ(foodMgr.place({0, 5}, body), PlacementResult::InvalidBounds);
    EXPECT_EQ(foodMgr.place({5, -1}, body), PlacementResult::InvalidBounds);
}

} // namespace
} // namespace snakegame

