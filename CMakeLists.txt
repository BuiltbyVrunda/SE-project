cmake_minimum_required(VERSION 3.20)
project(SnakeGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Collision tests can run without SFML or a graphical display.
add_library(snake_collision src/collision_detector.cpp)
target_include_directories(snake_collision PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

option(BUILD_GAME "Build the SFML game and movement module" ON)
if(BUILD_GAME)
    # Use SFML_DIR or CMAKE_PREFIX_PATH for non-standard installations.
    find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)
    add_library(snake src/snake.cpp)
    target_include_directories(snake PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
    target_link_libraries(snake PUBLIC sfml-graphics sfml-window sfml-system)

    add_executable(SnakeGame src/main.cpp)
    target_link_libraries(SnakeGame PRIVATE snake)
endif()

include(CTest)
if(BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG 52eb8108c5bdec04579160ae17225d66034bd723 # v1.17.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(collision_tests tests/collision_detector_test.cpp)
    target_link_libraries(collision_tests PRIVATE snake_collision GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(collision_tests)

    if(BUILD_GAME)
        add_executable(SnakeTests tests/snake_test.cpp)
        target_link_libraries(SnakeTests PRIVATE snake)
        add_test(NAME SnakeMovementTests COMMAND SnakeTests)
    endif()
endif()
