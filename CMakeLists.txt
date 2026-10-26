cmake_minimum_required(VERSION 3.20)

project(SnakeGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "C:/Libraries/SFML-2.6.2/lib/cmake/SFML")

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

add_library(snake
    src/snake.cpp
)

target_include_directories(snake PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(snake PUBLIC
    sfml-graphics
    sfml-window
    sfml-system
)

add_executable(SnakeGame
    src/main.cpp
)

target_link_libraries(SnakeGame PRIVATE
    snake
)

add_executable(SnakeTests
    tests/snake_test.cpp
)

target_link_libraries(SnakeTests PRIVATE
    snake
)

enable_testing()

add_test(
    NAME SnakeMovementTests
    COMMAND SnakeTests
)