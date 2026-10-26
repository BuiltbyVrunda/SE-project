#include <cassert>
#include <iostream>
#include "snake.h"

int main()
{
    Snake snake;

    // Test 1: Initial direction
    assert(snake.getDirection() == sf::Vector2f(20.f, 0.f));
    std::cout << "Test 1 passed: Initial direction\n";

    // Test 2: Valid direction change
    assert(snake.changeDirection({0.f, -20.f}));
    assert(snake.getDirection() == sf::Vector2f(0.f, -20.f));
    std::cout << "Test 2 passed: Valid direction change\n";

    // Test 3: Reject immediate reversal
    assert(!snake.changeDirection({0.f, 20.f}));
    std::cout << "Test 3 passed: Opposite direction rejected\n";

    // Test 4: Reject invalid direction
    snake.move();
    assert(!snake.changeDirection({10.f, 10.f}));
    std::cout << "Test 4 passed: Invalid direction rejected\n";

    std::cout << "\nAll tests passed!\n";

    return 0;
}