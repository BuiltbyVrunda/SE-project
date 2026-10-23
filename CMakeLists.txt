cmake_minimum_required(VERSION 3.14)
project(SnakeGame LANGUAGES CXX)

# Pure grid logic can be built and tested without SFML or a graphical display.
add_library(snake_collision src/collision_detector.cpp)
target_include_directories(snake_collision PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_features(snake_collision PUBLIC cxx_std_17)
set_target_properties(snake_collision PROPERTIES CXX_EXTENSIONS OFF)

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
endif()
