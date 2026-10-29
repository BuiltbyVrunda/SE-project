#include "snake.h"
#include <cmath>

Snake::Snake()
    : direction(20.f, 0.f),
      directionChangedThisStep(false)
{
    reset();
}

void Snake::reset()
{
    body.clear();
    direction = sf::Vector2f(20.f, 0.f);
    directionChangedThisStep = false;

    const float segmentSize = 20.f;
    const sf::Vector2f startPosition(100.f, 100.f);

    // Start with a small snake containing three segments (head at (100, 100)).
    for (int i = 0; i < 3; ++i)
    {
        sf::RectangleShape segment(sf::Vector2f(segmentSize, segmentSize));
        segment.setPosition(startPosition.x - i * segmentSize, startPosition.y);
        segment.setFillColor(sf::Color(70, 200, 100));
        body.push_back(segment);
    }

    // Give the head a distinct color.
    if (!body.empty())
    {
        body.front().setFillColor(sf::Color::Yellow);
    }
}

void Snake::advance(bool growSnake)
{
    if (body.empty())
    {
        return;
    }

    const float segmentSize = 20.f;
    const sf::Vector2f nextHeadPosition = body.front().getPosition() + direction;

    if (growSnake)
    {
        // Add a new head segment at nextHeadPosition
        sf::RectangleShape newHead(sf::Vector2f(segmentSize, segmentSize));
        newHead.setPosition(nextHeadPosition);
        newHead.setFillColor(sf::Color::Yellow);

        // Turn previous head green
        body.front().setFillColor(sf::Color(70, 200, 100));

        // Insert at beginning
        body.insert(body.begin(), newHead);
    }
    else
    {
        // Move segments from tail to second segment
        for (std::size_t i = body.size() - 1; i > 0; --i)
        {
            body[i].setPosition(body[i - 1].getPosition());
        }
        body.front().setPosition(nextHeadPosition);
    }

    // Allow next turn
    directionChangedThisStep = false;
}

void Snake::move()
{
    advance(false);
}

void Snake::grow()
{
    if (body.empty())
    {
        return;
    }

    // Add a segment at current tail position
    sf::RectangleShape segment(sf::Vector2f(20.f, 20.f));
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

snakegame::Body Snake::getGridBody() const
{
    snakegame::Body gridBody;
    gridBody.reserve(body.size());
    for (const auto& segment : body)
    {
        const sf::Vector2f pos = segment.getPosition();
        gridBody.push_back(snakegame::Cell{
            static_cast<int>(std::round(pos.x / 20.f)),
            static_cast<int>(std::round(pos.y / 20.f))
        });
    }
    return gridBody;
}

snakegame::Cell Snake::getNextHeadGrid() const
{
    if (body.empty())
    {
        return snakegame::Cell{0, 0};
    }
    const sf::Vector2f nextPos = body.front().getPosition() + direction;
    return snakegame::Cell{
        static_cast<int>(std::round(nextPos.x / 20.f)),
        static_cast<int>(std::round(nextPos.y / 20.f))
    };
}
