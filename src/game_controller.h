#ifndef SNAKE_GAME_CONTROLLER_H
#define SNAKE_GAME_CONTROLLER_H

#include "collision_detector.h"
#include "food_manager.h"
#include "grid_types.h"
#include "score_manager.h"
#include "snake.h"

namespace snakegame {

enum class GameState {
    Ready,
    Playing,
    GameOver,
    RoundComplete
};

class GameController {
public:
    explicit GameController(BoardBounds bounds = {40, 30}, float moveInterval = 0.15f);

    // Lifecycle actions
    void startRound();
    void restart();

    // Input handling (safely ignored if not Playing)
    bool handleDirectionInput(const sf::Vector2f& direction);

    // Updates timer and runs step when movement interval elapses
    void update(float dt);

    // Executes a single discrete movement step (returns false if fatal collision)
    bool step();

    // Accessors
    [[nodiscard]] GameState getState() const;
    [[nodiscard]] int getScore() const;
    [[nodiscard]] int getBestScore() const;
    [[nodiscard]] int getFoodsEaten() const;
    [[nodiscard]] BoardBounds getBounds() const;
    [[nodiscard]] float getMovementInterval() const;
    [[nodiscard]] CollisionType getLastCollision() const;

    [[nodiscard]] const Snake& getSnake() const;
    [[nodiscard]] Snake& getSnake();

    [[nodiscard]] const FoodManager& getFoodManager() const;
    [[nodiscard]] FoodManager& getFoodManager();

    [[nodiscard]] const ScoreManager& getScoreManager() const;

private:
    BoardBounds bounds;
    float movementInterval;
    float timeAccumulator;
    GameState state;
    CollisionType lastCollision;

    Snake snake;
    FoodManager foodManager;
    ScoreManager scoreManager;
    CollisionDetector collisionDetector;
};

} // namespace snakegame

#endif // SNAKE_GAME_CONTROLLER_H

