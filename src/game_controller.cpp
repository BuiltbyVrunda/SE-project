#include "game_controller.h"
#include <algorithm>

namespace snakegame {

GameController::GameController(BoardBounds bounds, float moveInterval)
    : bounds(bounds),
      movementInterval(moveInterval),
      timeAccumulator(0.f),
      state(GameState::Ready),
      lastCollision(CollisionType::None),
      snake(),
      foodManager(),
      scoreManager(),
      collisionDetector()
{
    // Pre-place food so it is visible in the Ready state
    foodManager.place(this->bounds, snake.getGridBody());
}

void GameController::startRound()
{
    if (state == GameState::Ready)
    {
        if (!foodManager.position().has_value())
        {
            foodManager.place(bounds, snake.getGridBody());
        }
        state = GameState::Playing;
        timeAccumulator = 0.f;
    }
}

void GameController::restart()
{
    snake.reset();
    scoreManager.resetRound(); // best score is kept across restarts
    timeAccumulator = 0.f;
    lastCollision = CollisionType::None;

    foodManager.reset();
    foodManager.place(bounds, snake.getGridBody());

    state = GameState::Playing;
}

bool GameController::handleDirectionInput(const sf::Vector2f& direction)
{
    if (state != GameState::Playing)
    {
        return false;
    }
    return snake.changeDirection(direction);
}

void GameController::update(float dt)
{
    if (state != GameState::Playing)
    {
        return;
    }

    timeAccumulator += dt;
    while (timeAccumulator >= movementInterval && state == GameState::Playing)
    {
        timeAccumulator -= movementInterval;
        step();
    }
}

bool GameController::step()
{
    if (state != GameState::Playing)
    {
        return false;
    }

    // 1. Calculate next head cell without committing movement
    const Cell nextHead = snake.getNextHeadGrid();

    // 2. Check if candidate cell has food
    const bool willGrow = foodManager.isAt(nextHead);

    // 3. Collision check before advancing
    const CollisionType col = collisionDetector.check(nextHead, snake.getGridBody(), bounds, willGrow);

    // 4. Handle collision
    if (col != CollisionType::None)
    {
        lastCollision = col;
        state = GameState::GameOver;
        return false;
    }

    // 5. Commit movement
    snake.advance(willGrow);

    // 6. Food consumption & score update
    if (willGrow)
    {
        scoreManager.addFoodPoints();
        const PlacementResult pres = foodManager.place(bounds, snake.getGridBody());
        if (pres == PlacementResult::BoardFull)
        {
            state = GameState::RoundComplete;
        }
    }

    return true;
}

GameState GameController::getState() const
{
    return state;
}

int GameController::getScore() const
{
    return scoreManager.getScore();
}

int GameController::getBestScore() const
{
    return scoreManager.getBestScore();
}

int GameController::getFoodsEaten() const
{
    return scoreManager.getFoodsEaten();
}

BoardBounds GameController::getBounds() const
{
    return bounds;
}

float GameController::getMovementInterval() const
{
    return movementInterval;
}

CollisionType GameController::getLastCollision() const
{
    return lastCollision;
}

const Snake& GameController::getSnake() const
{
    return snake;
}

Snake& GameController::getSnake()
{
    return snake;
}

const FoodManager& GameController::getFoodManager() const
{
    return foodManager;
}

FoodManager& GameController::getFoodManager()
{
    return foodManager;
}

const ScoreManager& GameController::getScoreManager() const
{
    return scoreManager;
}

} // namespace snakegame

