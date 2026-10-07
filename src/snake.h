#ifndef SNAKE_H
#define SNAKE_H

#include <SFML/Graphics.hpp>
#include <vector>

class Snake {
public:
    Snake();

private:
    std::vector<sf::RectangleShape> body;
    sf::Vector2f direction;
};

#endif
