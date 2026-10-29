#ifndef SNAKE_H
#define SNAKE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "grid_types.h"

class Snake
{
public:
    Snake();

    // Advances the snake by one step without growing.
    void move();

    // Appends a segment at the current tail position.
    void grow();

    // Advances the snake by one step, growing if grow is true.
    void advance(bool grow);

    // Resets snake to default initial state (3 segments, moving right).
    void reset();

    // Changes direction if valid and not reversed or already changed this tick.
    bool changeDirection(const sf::Vector2f& newDirection);

    // Accessors
    const std::vector<sf::RectangleShape>& getBody() const;
    sf::Vector2f getDirection() const;

    // Grid conversion helpers
    [[nodiscard]] snakegame::Body getGridBody() const;
    [[nodiscard]] snakegame::Cell getNextHeadGrid() const;

private:
    std::vector<sf::RectangleShape> body;
    sf::Vector2f direction;
    bool directionChangedThisStep = false;
};

#endif // SNAKE_H
