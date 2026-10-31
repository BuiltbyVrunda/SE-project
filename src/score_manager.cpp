#include "score_manager.h"

namespace snakegame {

ScoreManager::ScoreManager(int pointsPerFood)
    : pointsPerFood(pointsPerFood > 0 ? pointsPerFood : DefaultPointsPerFood),
      score(0),
      bestScore(0),
      foodsEaten(0)
{
}

void ScoreManager::addFoodPoints()
{
    ++foodsEaten;
    score += pointsPerFood;
    if (score > bestScore)
    {
        bestScore = score;
    }
}

void ScoreManager::resetRound()
{
    score = 0;
    foodsEaten = 0;
}

int ScoreManager::getScore() const
{
    return score;
}

int ScoreManager::getBestScore() const
{
    return bestScore;
}

int ScoreManager::getFoodsEaten() const
{
    return foodsEaten;
}

int ScoreManager::getPointsPerFood() const
{
    return pointsPerFood;
}

} // namespace snakegame
