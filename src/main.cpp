#include <SFML/Graphics.hpp>
#include "snake.h"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(800, 600),
        "Snake Game"
    );

    window.setFramerateLimit(60);

    Snake snake;
    sf::Clock movementClock;

    const float movementInterval = 0.15f;

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
                    snake.changeDirection({ 0.f, -20.f });
                    break;

                case sf::Keyboard::Down:
                    snake.changeDirection({ 0.f, 20.f });
                    break;

                case sf::Keyboard::Left:
                    snake.changeDirection({ -20.f, 0.f });
                    break;

                case sf::Keyboard::Right:
                    snake.changeDirection({ 20.f, 0.f });
                    break;

                default:
                    // Unsupported keys are safely ignored.
                    break;
                }
            }
        }

        if (movementClock.getElapsedTime().asSeconds() >= movementInterval)
        {
            snake.move();
            movementClock.restart();
        }

        window.clear(sf::Color::Black);

        for (const auto& segment : snake.getBody())
        {
            window.draw(segment);
        }

        window.display();
    }

    return 0;
}