#ifndef SNAKE_GAME_SCORE_MANAGER_H
#define SNAKE_GAME_SCORE_MANAGER_H

namespace snakegame {

// Tracks the score for the current round and the best score of the session.
// Pure game logic with no SFML dependency, so it can be unit-tested headlessly.
// Owner: Vrunda C (food generation and scoring module, FR-10, FR-11).
class ScoreManager {
public:
    static constexpr int DefaultPointsPerFood = 10;

    explicit ScoreManager(int pointsPerFood = DefaultPointsPerFood);

    // Records one food item eaten: adds the configured points to the score
    // and updates the best score if the current score is higher.
    void addFoodPoints();

    // Starts a new round: score and food count go back to 0.
    // The best score is kept for the rest of the session.
    void resetRound();

    [[nodiscard]] int getScore() const;
    [[nodiscard]] int getBestScore() const;
    [[nodiscard]] int getFoodsEaten() const;
    [[nodiscard]] int getPointsPerFood() const;

private:
    int pointsPerFood;
    int score;
    int bestScore;
    int foodsEaten;
};

} // namespace snakegame

#endif // SNAKE_GAME_SCORE_MANAGER_H
