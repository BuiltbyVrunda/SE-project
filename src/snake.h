#ifndef SNAKE_H
#define SNAKE_H

#include <SFML/Graphics.hpp>
#include <vector>

class Snake {
public:
    Snake();

    void move();

    bool changeDirection(const sf::Vector2f& newDirection);

    const std::vector<sf::RectangleShape>& getBody() const;
    sf::Vector2f getDirection() const;

private:
    std::vector<sf::RectangleShape> body;
    sf::Vector2f direction;
    bool directionChangedThisStep = false;
};

#endif