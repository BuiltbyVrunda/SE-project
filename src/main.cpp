#include <SFML/Graphics.hpp>
#include "snake.h"

int main()
{
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;

    sf::RenderWindow window(
        sf::VideoMode(windowWidth, windowHeight),
        "Snake Game"
    );

    window.setFramerateLimit(60);

    Snake snake;
    sf::Clock movementClock;

    const sf::Time movementInterval =
        sf::milliseconds(150);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::Up:
                    snake.changeDirection({0.f, -20.f});
                    break;

                case sf::Keyboard::Down:
                    snake.changeDirection({0.f, 20.f});
                    break;

                case sf::Keyboard::Left:
                    snake.changeDirection({-20.f, 0.f});
                    break;

                case sf::Keyboard::Right:
                    snake.changeDirection({20.f, 0.f});
                    break;

                default:
                    // Ignore unsupported keys.
                    break;
                }
            }
        }

        // Move the snake at regular intervals.
        if (movementClock.getElapsedTime() >= movementInterval)
        {
            snake.move();
            movementClock.restart();
        }

        // Draw the game.
        window.clear(sf::Color::Black);

        for (const auto& segment : snake.getBody())
        {
            window.draw(segment);
        }

        window.display();
    }

    return 0;
}