#include <cstdlib>
#include <iostream>
#include "snake.h"

// CHECK() is compiled out when NDEBUG is defined (every Release build, including CI),
// which would make these tests pass without checking anything. CHECK always runs.
#define CHECK(condition)                                                              \
    do                                                                                \
    {                                                                                 \
        if (!(condition))                                                             \
        {                                                                             \
            std::cerr << "CHECK failed: " #condition " (" __FILE__ ":" << __LINE__    \
                      << ")\n";                                                       \
            std::exit(EXIT_FAILURE);                                                  \
        }                                                                             \
    } while (false)

int main()
{
    // Test 1: Initial direction
    Snake snake1;
    CHECK(snake1.getDirection() == sf::Vector2f(20.f, 0.f));
    std::cout << "Test 1 passed: Initial direction\n";

    // Test 2: Valid direction change
    Snake snake2;
    CHECK(snake2.changeDirection({0.f, -20.f}));
    CHECK(snake2.getDirection() == sf::Vector2f(0.f, -20.f));
    std::cout << "Test 2 passed: Valid direction change\n";

    // Test 3: Direct reversal rejected
    Snake snake3;
    CHECK(!snake3.changeDirection({-20.f, 0.f}));
    CHECK(snake3.getDirection() == sf::Vector2f(20.f, 0.f));
    std::cout << "Test 3 passed: Direct reversal rejected\n";

    // Test 4: Rapid consecutive turn rejected
    Snake snake4;
    CHECK(snake4.changeDirection({0.f, -20.f}));
    CHECK(!snake4.changeDirection({-20.f, 0.f}));
    CHECK(snake4.getDirection() == sf::Vector2f(0.f, -20.f));
    std::cout << "Test 4 passed: Rapid consecutive turn rejected\n";

    // Test 5: Invalid directions rejected
    Snake snake5;
    CHECK(!snake5.changeDirection({10.f, 10.f}));
    CHECK(!snake5.changeDirection({0.f, 0.f}));
    CHECK(snake5.getDirection() == sf::Vector2f(20.f, 0.f));
    std::cout << "Test 5 passed: Invalid directions rejected\n";

    // Test 6: Snake moves forward
    Snake snake6;
    sf::Vector2f initialPosition =
        snake6.getBody().front().getPosition();

    snake6.move();

    sf::Vector2f newPosition =
        snake6.getBody().front().getPosition();

    CHECK(newPosition == sf::Vector2f(
        initialPosition.x + 20.f,
        initialPosition.y
    ));

    std::cout << "Test 6 passed: Snake moves forward\n";

    std::cout << "\nAll 6 tests passed!\n";
    return 0;
}