#include "snake.h"

Snake::Snake()
    : direction(20.f, 0.f),
      directionChangedThisStep(false)
{
    const float segmentSize = 20.f;
    const sf::Vector2f startPosition(100.f, 100.f);

    // Start with a small snake containing three segments.
    for (int i = 0; i < 3; ++i)
    {
        sf::RectangleShape segment(
            sf::Vector2f(segmentSize, segmentSize)
        );

        segment.setPosition(
            startPosition.x - i * segmentSize,
            startPosition.y
        );

        segment.setFillColor(sf::Color(70, 200, 100));
        body.push_back(segment);
    }

    // Give the head a distinct color.
    if (!body.empty())
    {
        body.front().setFillColor(sf::Color::Yellow);
    }
}

void Snake::move()
{
    if (body.empty())
    {
        return;
    }

    // Save the current tail position.
    sf::Vector2f previousTailPosition =
        body.back().getPosition();

    // Move each segment to the previous segment's position.
    for (std::size_t i = body.size() - 1; i > 0; --i)
    {
        body[i].setPosition(body[i - 1].getPosition());
    }

    // Move the head.
    sf::Vector2f nextPosition =
        body.front().getPosition() + direction;

    const float segmentSize = 20.f;
    const float windowWidth = 800.f;
    const float windowHeight = 600.f;

    // Wrap around the horizontal edges.
    if (nextPosition.x >= windowWidth)
    {
        nextPosition.x = 0.f;
    }
    else if (nextPosition.x < 0.f)
    {
        nextPosition.x = windowWidth - segmentSize;
    }

    // Wrap around the vertical edges.
    if (nextPosition.y >= windowHeight)
    {
        nextPosition.y = 0.f;
    }
    else if (nextPosition.y < 0.f)
    {
        nextPosition.y = windowHeight - segmentSize;
    }

    body.front().setPosition(nextPosition);

    // Allow the next turn.
    directionChangedThisStep = false;
}

void Snake::grow()
{
    if (body.empty())
    {
        return;
    }

    // Add a segment at the current tail position.
    sf::RectangleShape segment(
        sf::Vector2f(20.f, 20.f)
    );

    segment.setPosition(body.back().getPosition());
    segment.setFillColor(sf::Color(70, 200, 100));

    body.push_back(segment);
}

bool Snake::changeDirection(const sf::Vector2f& newDirection)
{
    if (directionChangedThisStep)
    {
        return false;
    }

    const bool validHorizontal =
        (newDirection.x == 20.f || newDirection.x == -20.f) &&
        newDirection.y == 0.f;

    const bool validVertical =
        (newDirection.y == 20.f || newDirection.y == -20.f) &&
        newDirection.x == 0.f;

    if (!validHorizontal && !validVertical)
    {
        return false;
    }

    // Reject immediate reversals.
    if (newDirection.x == -direction.x &&
        newDirection.y == -direction.y)
    {
        return false;
    }

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