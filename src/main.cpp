#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "game_controller.h"

namespace {

bool loadGameFont(sf::Font& font)
{
    const std::vector<std::string> candidatePaths = {
        "assets/arial.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/consola.ttf",
        "C:/Windows/Fonts/segoeui.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        "/usr/share/fonts/TTF/DejaVuSans.ttf"
    };

    for (const auto& path : candidatePaths)
    {
        if (font.loadFromFile(path))
        {
            return true;
        }
    }
    return false;
}

} // namespace

int main()
{
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;
    const float cellSize = 20.f;

    sf::RenderWindow window(
        sf::VideoMode(windowWidth, windowHeight),
        "Snake Game"
    );
    window.setFramerateLimit(60);

    snakegame::GameController controller({40, 30}, 0.15f);

    sf::Font font;
    const bool fontLoaded = loadGameFont(font);
    if (!fontLoaded)
    {
        std::cerr << "Note: System font could not be loaded; text rendering will be minimal.\n";
    }

    sf::Clock frameClock;

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
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space)
                {
                    if (controller.getState() == snakegame::GameState::Ready)
                    {
                        controller.startRound();
                    }
                }
                else if (event.key.code == sf::Keyboard::R)
                {
                    if (controller.getState() == snakegame::GameState::GameOver ||
                        controller.getState() == snakegame::GameState::RoundComplete)
                    {
                        controller.restart();
                    }
                }
                else if (controller.getState() == snakegame::GameState::Playing)
                {
                    switch (event.key.code)
                    {
                    case sf::Keyboard::Up:
                        controller.handleDirectionInput({ 0.f, -cellSize });
                        break;
                    case sf::Keyboard::Down:
                        controller.handleDirectionInput({ 0.f, cellSize });
                        break;
                    case sf::Keyboard::Left:
                        controller.handleDirectionInput({ -cellSize, 0.f });
                        break;
                    case sf::Keyboard::Right:
                        controller.handleDirectionInput({ cellSize, 0.f });
                        break;
                    default:
                        // Unsupported keys safely ignored (SR-01)
                        break;
                    }
                }
            }
        }

        // Advance simulation with frame delta (cap to avoid spiral after window movement)
        const float dt = std::min(frameClock.restart().asSeconds(), 0.2f);
        controller.update(dt);

        window.clear(sf::Color(18, 18, 18));

        // Draw play area boundary
        sf::RectangleShape border(sf::Vector2f(windowWidth - 2.f, windowHeight - 2.f));
        border.setPosition(1.f, 1.f);
        border.setFillColor(sf::Color::Transparent);
        border.setOutlineThickness(1.f);
        border.setOutlineColor(sf::Color(60, 60, 60));
        window.draw(border);

        // Draw food
        const auto foodPos = controller.getFoodManager().position();
        if (foodPos.has_value())
        {
            sf::CircleShape foodShape(cellSize * 0.45f);
            foodShape.setPosition(foodPos->x * cellSize + cellSize * 0.05f,
                                  foodPos->y * cellSize + cellSize * 0.05f);
            foodShape.setFillColor(sf::Color(230, 50, 50));
            window.draw(foodShape);
        }

        // Draw snake
        for (const auto& segment : controller.getSnake().getBody())
        {
            window.draw(segment);
        }

        // Draw UI and HUD
        if (fontLoaded)
        {
            if (controller.getState() == snakegame::GameState::Playing)
            {
                sf::Text scoreText;
                scoreText.setFont(font);
                scoreText.setCharacterSize(20);
                scoreText.setFillColor(sf::Color::White);
                scoreText.setString("Score: " + std::to_string(controller.getScore()));
                scoreText.setPosition(15.f, 10.f);
                window.draw(scoreText);

                sf::Text bestText;
                bestText.setFont(font);
                bestText.setCharacterSize(20);
                bestText.setFillColor(sf::Color(255, 215, 0));
                bestText.setString("Best: " + std::to_string(controller.getBestScore()));
                const auto bestBounds = bestText.getLocalBounds();
                bestText.setPosition(windowWidth - bestBounds.width - 15.f, 10.f);
                window.draw(bestText);
            }
            else if (controller.getState() == snakegame::GameState::Ready)
            {
                sf::RectangleShape overlay(sf::Vector2f(500.f, 260.f));
                overlay.setOrigin(250.f, 130.f);
                overlay.setPosition(windowWidth / 2.f, windowHeight / 2.f);
                overlay.setFillColor(sf::Color(0, 0, 0, 210));
                overlay.setOutlineThickness(2.f);
                overlay.setOutlineColor(sf::Color(70, 200, 100));
                window.draw(overlay);

                sf::Text titleText;
                titleText.setFont(font);
                titleText.setCharacterSize(36);
                titleText.setFillColor(sf::Color::Yellow);
                titleText.setStyle(sf::Text::Bold);
                titleText.setString("SNAKE GAME");
                const auto titleBounds = titleText.getLocalBounds();
                titleText.setPosition(windowWidth / 2.f - titleBounds.width / 2.f,
                                      windowHeight / 2.f - 100.f);
                window.draw(titleText);

                sf::Text promptText;
                promptText.setFont(font);
                promptText.setCharacterSize(22);
                promptText.setFillColor(sf::Color::White);
                promptText.setString("Press ENTER or SPACE to Start");
                const auto promptBounds = promptText.getLocalBounds();
                promptText.setPosition(windowWidth / 2.f - promptBounds.width / 2.f,
                                       windowHeight / 2.f - 30.f);
                window.draw(promptText);

                sf::Text controlsText;
                controlsText.setFont(font);
                controlsText.setCharacterSize(16);
                controlsText.setFillColor(sf::Color(180, 180, 180));
                controlsText.setString("Controls: Arrow Keys to Move | ESC to Exit");
                const auto controlsBounds = controlsText.getLocalBounds();
                controlsText.setPosition(windowWidth / 2.f - controlsBounds.width / 2.f,
                                         windowHeight / 2.f + 30.f);
                window.draw(controlsText);

                sf::Text tipText;
                tipText.setFont(font);
                tipText.setCharacterSize(14);
                tipText.setFillColor(sf::Color(140, 140, 140));
                tipText.setString("Eat red food to grow | Avoid walls & self-collision");
                const auto tipBounds = tipText.getLocalBounds();
                tipText.setPosition(windowWidth / 2.f - tipBounds.width / 2.f,
                                    windowHeight / 2.f + 65.f);
                window.draw(tipText);
            }
            else if (controller.getState() == snakegame::GameState::GameOver)
            {
                sf::RectangleShape overlay(sf::Vector2f(500.f, 260.f));
                overlay.setOrigin(250.f, 130.f);
                overlay.setPosition(windowWidth / 2.f, windowHeight / 2.f);
                overlay.setFillColor(sf::Color(0, 0, 0, 220));
                overlay.setOutlineThickness(2.f);
                overlay.setOutlineColor(sf::Color(220, 50, 50));
                window.draw(overlay);

                sf::Text overText;
                overText.setFont(font);
                overText.setCharacterSize(38);
                overText.setFillColor(sf::Color(240, 60, 60));
                overText.setStyle(sf::Text::Bold);
                overText.setString("GAME OVER");
                const auto overBounds = overText.getLocalBounds();
                overText.setPosition(windowWidth / 2.f - overBounds.width / 2.f,
                                     windowHeight / 2.f - 105.f);
                window.draw(overText);

                std::string reason = "Collision Detected!";
                if (controller.getLastCollision() == snakegame::CollisionType::Boundary)
                {
                    reason = "Wall Collision!";
                }
                else if (controller.getLastCollision() == snakegame::CollisionType::Self)
                {
                    reason = "Self-Collision!";
                }
                sf::Text reasonText;
                reasonText.setFont(font);
                reasonText.setCharacterSize(18);
                reasonText.setFillColor(sf::Color(220, 180, 180));
                reasonText.setString(reason);
                const auto reasonBounds = reasonText.getLocalBounds();
                reasonText.setPosition(windowWidth / 2.f - reasonBounds.width / 2.f,
                                       windowHeight / 2.f - 55.f);
                window.draw(reasonText);

                sf::Text scoreText;
                scoreText.setFont(font);
                scoreText.setCharacterSize(24);
                scoreText.setFillColor(sf::Color::Yellow);
                scoreText.setString("Final Score: " + std::to_string(controller.getScore()));
                const auto scoreBounds = scoreText.getLocalBounds();
                scoreText.setPosition(windowWidth / 2.f - scoreBounds.width / 2.f,
                                      windowHeight / 2.f - 15.f);
                window.draw(scoreText);

                sf::Text bestText;
                bestText.setFont(font);
                bestText.setCharacterSize(18);
                bestText.setFillColor(sf::Color(255, 215, 0));
                bestText.setString("Best Score: " + std::to_string(controller.getBestScore()));
                const auto bestBounds = bestText.getLocalBounds();
                bestText.setPosition(windowWidth / 2.f - bestBounds.width / 2.f,
                                     windowHeight / 2.f + 18.f);
                window.draw(bestText);

                sf::Text restartText;
                restartText.setFont(font);
                restartText.setCharacterSize(20);
                restartText.setFillColor(sf::Color::White);
                restartText.setString("Press 'R' to Restart");
                const auto restartBounds = restartText.getLocalBounds();
                restartText.setPosition(windowWidth / 2.f - restartBounds.width / 2.f,
                                        windowHeight / 2.f + 50.f);
                window.draw(restartText);

                sf::Text exitText;
                exitText.setFont(font);
                exitText.setCharacterSize(15);
                exitText.setFillColor(sf::Color(160, 160, 160));
                exitText.setString("Press ESC to Exit");
                const auto exitBounds = exitText.getLocalBounds();
                exitText.setPosition(windowWidth / 2.f - exitBounds.width / 2.f,
                                     windowHeight / 2.f + 88.f);
                window.draw(exitText);
            }
            else if (controller.getState() == snakegame::GameState::RoundComplete)
            {
                sf::RectangleShape overlay(sf::Vector2f(500.f, 220.f));
                overlay.setOrigin(250.f, 110.f);
                overlay.setPosition(windowWidth / 2.f, windowHeight / 2.f);
                overlay.setFillColor(sf::Color(0, 0, 0, 220));
                overlay.setOutlineThickness(2.f);
                overlay.setOutlineColor(sf::Color::Yellow);
                window.draw(overlay);

                sf::Text winText;
                winText.setFont(font);
                winText.setCharacterSize(32);
                winText.setFillColor(sf::Color::Yellow);
                winText.setStyle(sf::Text::Bold);
                winText.setString("BOARD FULL! YOU WIN!");
                const auto winBounds = winText.getLocalBounds();
                winText.setPosition(windowWidth / 2.f - winBounds.width / 2.f,
                                    windowHeight / 2.f - 80.f);
                window.draw(winText);

                sf::Text scoreText;
                scoreText.setFont(font);
                scoreText.setCharacterSize(24);
                scoreText.setFillColor(sf::Color::White);
                scoreText.setString("Final Score: " + std::to_string(controller.getScore()));
                const auto scoreBounds = scoreText.getLocalBounds();
                scoreText.setPosition(windowWidth / 2.f - scoreBounds.width / 2.f,
                                      windowHeight / 2.f - 20.f);
                window.draw(scoreText);

                sf::Text bestText;
                bestText.setFont(font);
                bestText.setCharacterSize(18);
                bestText.setFillColor(sf::Color(255, 215, 0));
                bestText.setString("Best Score: " + std::to_string(controller.getBestScore()));
                const auto bestBounds = bestText.getLocalBounds();
                bestText.setPosition(windowWidth / 2.f - bestBounds.width / 2.f,
                                     windowHeight / 2.f + 14.f);
                window.draw(bestText);

                sf::Text restartText;
                restartText.setFont(font);
                restartText.setCharacterSize(20);
                restartText.setFillColor(sf::Color::Green);
                restartText.setString("Press 'R' to Restart");
                const auto restartBounds = restartText.getLocalBounds();
                restartText.setPosition(windowWidth / 2.f - restartBounds.width / 2.f,
                                        windowHeight / 2.f + 50.f);
                window.draw(restartText);
            }
        }

        window.display();
    }

    return 0;
}
