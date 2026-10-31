#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "game_controller.h"

namespace
{
    constexpr unsigned int CELL_SIZE = 20;
    constexpr unsigned int WINDOW_WIDTH = 800;
    constexpr unsigned int WINDOW_HEIGHT = 600;

    bool loadGameFont(sf::Font& font)
    {
        const std::string fontPaths[] = {
            "assets/arial.ttf",
            "C:/Windows/Fonts/arial.ttf",
            "C:/Windows/Fonts/segoeui.ttf",
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"
        };

        for (const auto& path : fontPaths)
        {
            if (font.loadFromFile(path))
            {
                return true;
            }
        }

        return false;
    }

    void drawText(sf::RenderWindow& window,
                  const sf::Font& font,
                  const std::string& message,
                  unsigned int size,
                  float x,
                  float y,
                  sf::Color color = sf::Color::White)
    {
        sf::Text text(message, font, size);
        text.setFillColor(color);
        text.setPosition(x, y);
        window.draw(text);
    }

    void drawCenteredText(sf::RenderWindow& window,
                          const sf::Font& font,
                          const std::string& message,
                          unsigned int size,
                          float y,
                          sf::Color color = sf::Color::White)
    {
        sf::Text text(message, font, size);
        text.setFillColor(color);

        const sf::FloatRect textBounds = text.getLocalBounds();

        text.setOrigin(
            textBounds.left + textBounds.width / 2.f,
            textBounds.top + textBounds.height / 2.f
        );

        text.setPosition(WINDOW_WIDTH / 2.f, y);
        window.draw(text);
    }

    void drawSnake(sf::RenderWindow& window, const Snake& snake)
    {
        const auto& body = snake.getBody();

        for (std::size_t i = 0; i < body.size(); ++i)
        {
            sf::RectangleShape segment = body[i];

            segment.setFillColor(
                i == 0 ? sf::Color(255, 210, 50)
                       : sf::Color(60, 190, 90)
            );

            segment.setOutlineThickness(1.f);
            segment.setOutlineColor(sf::Color(25, 25, 25));

            window.draw(segment);
        }
    }

    void drawFood(sf::RenderWindow& window,
                  const snakegame::FoodManager& foodManager)
    {
        const auto food = foodManager.position();

        if (!food.has_value())
        {
            return;
        }

        sf::CircleShape foodShape(CELL_SIZE / 2.f - 2.f);

        foodShape.setFillColor(sf::Color::Red);

        foodShape.setPosition(
            static_cast<float>(food->x * CELL_SIZE + 2),
            static_cast<float>(food->y * CELL_SIZE + 2)
        );

        window.draw(foodShape);
    }
}

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "Snake Game"
    );

    window.setFramerateLimit(60);

    sf::Font font;
    const bool fontLoaded = loadGameFont(font);

    snakegame::GameController controller({40, 30}, 0.15f);

    sf::Clock deltaClock;

    // FPS measurement variables
    sf::Clock fpsClock;
    unsigned int frameCount = 0;
    float currentFPS = 0.f;

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }

                // Start the game.
                if ((event.key.code == sf::Keyboard::Enter ||
                     event.key.code == sf::Keyboard::Space) &&
                    controller.getState() == snakegame::GameState::Ready)
                {
                    controller.startRound();
                }

                // Restart after Game Over or Round Complete.
                if (event.key.code == sf::Keyboard::R &&
                    (controller.getState() == snakegame::GameState::GameOver ||
                     controller.getState() == snakegame::GameState::RoundComplete))
                {
                    controller.restart();
                }

                // Arrow-key movement.
                // Snake expects movement values of 20 or -20.
                if (controller.getState() == snakegame::GameState::Playing)
                {
                    switch (event.key.code)
                    {
                        case sf::Keyboard::Up:
                            controller.handleDirectionInput({0.f, -20.f});
                            break;

                        case sf::Keyboard::Down:
                            controller.handleDirectionInput({0.f, 20.f});
                            break;

                        case sf::Keyboard::Left:
                            controller.handleDirectionInput({-20.f, 0.f});
                            break;

                        case sf::Keyboard::Right:
                            controller.handleDirectionInput({20.f, 0.f});
                            break;

                        default:
                            break;
                    }
                }
            }
        }

        // Update game using elapsed time.
        const float deltaTime = deltaClock.restart().asSeconds();
        controller.update(deltaTime);

        // Calculate FPS every second.
        ++frameCount;

        const float fpsElapsed = fpsClock.getElapsedTime().asSeconds();

        if (fpsElapsed >= 1.f)
        {
            currentFPS = static_cast<float>(frameCount) / fpsElapsed;

            frameCount = 0;
            fpsClock.restart();
        }

        window.clear(sf::Color(25, 25, 25));

        // Draw vertical grid lines.
        for (unsigned int x = 0; x <= WINDOW_WIDTH; x += CELL_SIZE)
        {
            sf::Vertex line[] = {
                sf::Vertex(
                    sf::Vector2f(static_cast<float>(x), 0.f),
                    sf::Color(45, 45, 45)
                ),
                sf::Vertex(
                    sf::Vector2f(
                        static_cast<float>(x),
                        static_cast<float>(WINDOW_HEIGHT)
                    ),
                    sf::Color(45, 45, 45)
                )
            };

            window.draw(line, 2, sf::Lines);
        }

        // Draw horizontal grid lines.
        for (unsigned int y = 0; y <= WINDOW_HEIGHT; y += CELL_SIZE)
        {
            sf::Vertex line[] = {
                sf::Vertex(
                    sf::Vector2f(0.f, static_cast<float>(y)),
                    sf::Color(45, 45, 45)
                ),
                sf::Vertex(
                    sf::Vector2f(
                        static_cast<float>(WINDOW_WIDTH),
                        static_cast<float>(y)
                    ),
                    sf::Color(45, 45, 45)
                )
            };

            window.draw(line, 2, sf::Lines);
        }

        // Draw food and snake.
        drawFood(window, controller.getFoodManager());
        drawSnake(window, controller.getSnake());

        if (fontLoaded)
        {
            // Score display.
            drawText(
                window,
                font,
                "Score: " + std::to_string(controller.getScore()),
                20,
                10.f,
                10.f
            );

            drawText(
                window,
                font,
                "Best: " + std::to_string(controller.getBestScore()),
                20,
                10.f,
                35.f
            );

            // FPS display in the top-right corner.
            drawText(
                window,
                font,
                "FPS: " + std::to_string(
                    static_cast<int>(currentFPS)
                ),
                20,
                700.f,
                10.f,
                sf::Color::Green
            );

            // Ready screen.
            if (controller.getState() == snakegame::GameState::Ready)
            {
                sf::RectangleShape overlay(sf::Vector2f(460.f, 220.f));

                overlay.setFillColor(sf::Color(0, 0, 0, 210));
                overlay.setPosition(170.f, 190.f);

                window.draw(overlay);

                drawCenteredText(
                    window, font, "SNAKE GAME",
                    42, 245.f, sf::Color::Yellow
                );

                drawCenteredText(
                    window, font, "Press ENTER or SPACE to Start",
                    20, 315.f
                );

                drawCenteredText(
                    window, font, "Use Arrow Keys to Move",
                    18, 355.f
                );

                drawCenteredText(
                    window, font, "Eat food and avoid collisions!",
                    16, 390.f, sf::Color::Green
                );
            }

            // Game Over screen.
            if (controller.getState() == snakegame::GameState::GameOver)
            {
                sf::RectangleShape overlay(sf::Vector2f(460.f, 260.f));

                overlay.setFillColor(sf::Color(0, 0, 0, 220));
                overlay.setPosition(170.f, 170.f);

                window.draw(overlay);

                drawCenteredText(
                    window, font, "GAME OVER",
                    42, 225.f, sf::Color::Red
                );

                std::string reason = "Collision detected";

                if (controller.getLastCollision() ==
                    snakegame::CollisionType::Boundary)
                {
                    reason = "You hit the boundary!";
                }
                else if (controller.getLastCollision() ==
                         snakegame::CollisionType::Self)
                {
                    reason = "You hit yourself!";
                }

                drawCenteredText(
                    window, font, reason,
                    20, 285.f
                );

                drawCenteredText(
                    window, font,
                    "Final Score: " + std::to_string(controller.getScore()),
                    22, 325.f, sf::Color::Yellow
                );

                drawCenteredText(
                    window, font,
                    "Best Score: " + std::to_string(controller.getBestScore()),
                    18, 360.f
                );

                drawCenteredText(
                    window, font, "Press R to Restart",
                    18, 400.f
                );

                drawCenteredText(
                    window, font, "Press ESC to Exit",
                    16, 430.f
                );
            }

            // Round completed screen.
            if (controller.getState() == snakegame::GameState::RoundComplete)
            {
                sf::RectangleShape overlay(sf::Vector2f(460.f, 220.f));

                overlay.setFillColor(sf::Color(0, 0, 0, 220));
                overlay.setPosition(170.f, 190.f);

                window.draw(overlay);

                drawCenteredText(
                    window, font, "ROUND COMPLETE!",
                    36, 250.f, sf::Color::Green
                );

                drawCenteredText(
                    window, font,
                    "Final Score: " + std::to_string(controller.getScore()),
                    22, 315.f, sf::Color::Yellow
                );

                drawCenteredText(
                    window, font, "Press R to Play Again",
                    18, 365.f
                );
            }
        }

        window.display();
    }

    return 0;
}