#include "snake.h"

Snake::Snake()
    : direction(20.f, 0.f),
      directionChangedThisStep(false)
{
    sf::RectangleShape segment(sf::Vector2f(20.f, 20.f));
    segment.setPosition(sf::Vector2f(100.f, 100.f));
    body.push_back(segment);
}

void Snake::move()
{
    sf::Vector2f nextPosition =
        body.front().getPosition() + direction;

    for (std::size_t i = body.size() - 1; i > 0; --i)
    {
        body[i].setPosition(body[i - 1].getPosition());
    }

    body.front().setPosition(nextPosition);
    directionChangedThisStep = false;
}

bool Snake::changeDirection(const sf::Vector2f& newDirection)
{
    if (directionChangedThisStep)
        return false;

    const bool validHorizontal =
        (newDirection.x == 20.f || newDirection.x == -20.f) &&
        newDirection.y == 0.f;

    const bool validVertical =
        (newDirection.y == 20.f || newDirection.y == -20.f) &&
        newDirection.x == 0.f;

    if (!validHorizontal && !validVertical)
        return false;

    if (newDirection.x == -direction.x &&
        newDirection.y == -direction.y)
        return false;

    direction = newDirection;
    directionChangedThisStep = true;
    return true;
}

const std::vector<sf::RectangleShape>& Snake::getBody() const
{
    return body;
}

sf::Vector2f Snake::getDirection() const
{
    return direction;
}