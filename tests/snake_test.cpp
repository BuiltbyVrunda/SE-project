#include <cassert>
#include <iostream>
#include "snake.h"

int main()
{
    // Test 1: Initial direction
    Snake snake1;
    assert(snake1.getDirection() == sf::Vector2f(20.f, 0.f));
    std::cout << "Test 1 passed: Initial direction\n";

    // Test 2: Valid direction change
    Snake snake2;
    assert(snake2.changeDirection({0.f, -20.f}));
    assert(snake2.getDirection() == sf::Vector2f(0.f, -20.f));
    std::cout << "Test 2 passed: Valid direction change\n";

    // Test 3: Direct reversal rejected
    Snake snake3;
    assert(!snake3.changeDirection({-20.f, 0.f}));
    assert(snake3.getDirection() == sf::Vector2f(20.f, 0.f));
    std::cout << "Test 3 passed: Direct reversal rejected\n";

    // Test 4: Rapid consecutive turn rejected
    Snake snake4;
    assert(snake4.changeDirection({0.f, -20.f}));
    assert(!snake4.changeDirection({-20.f, 0.f}));
    assert(snake4.getDirection() == sf::Vector2f(0.f, -20.f));
    std::cout << "Test 4 passed: Rapid consecutive turn rejected\n";

    // Test 5: Invalid directions rejected
    Snake snake5;
    assert(!snake5.changeDirection({10.f, 10.f}));
    assert(!snake5.changeDirection({0.f, 0.f}));
    assert(snake5.getDirection() == sf::Vector2f(20.f, 0.f));
    std::cout << "Test 5 passed: Invalid directions rejected\n";

    // Test 6: Snake moves forward
    Snake snake6;
    sf::Vector2f initialPosition =
        snake6.getBody().front().getPosition();

    snake6.move();

    sf::Vector2f newPosition =
        snake6.getBody().front().getPosition();

    assert(newPosition == sf::Vector2f(
        initialPosition.x + 20.f,
        initialPosition.y
    ));

    std::cout << "Test 6 passed: Snake moves forward\n";

    std::cout << "\nAll 6 tests passed!\n";
    return 0;
}