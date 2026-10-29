cmake_minimum_required(VERSION 3.20)
project(SnakeGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Link GCC runtime statically on MinGW to prevent missing runtime DLLs
if(MINGW)
    add_link_options(-static-libgcc -static-libstdc++)
endif()

# Core domain libraries (independent of graphical display)
add_library(snake_collision src/collision_detector.cpp)
target_include_directories(snake_collision PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(snake_food src/food_manager.cpp)
target_include_directories(snake_food PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

option(BUILD_GAME "Build the SFML game and movement module" ON)
if(BUILD_GAME)
    # Use SFML_DIR or CMAKE_PREFIX_PATH for non-standard installations.
    find_package(SFML 2 COMPONENTS graphics window system REQUIRED)

    add_library(snake src/snake.cpp)
    target_include_directories(snake PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
    target_link_libraries(snake PUBLIC sfml-graphics sfml-window sfml-system)

    add_library(snake_controller src/game_controller.cpp)
    target_include_directories(snake_controller PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
    target_link_libraries(snake_controller PUBLIC snake snake_food snake_collision)

    add_executable(SnakeGame src/main.cpp)
    target_link_libraries(SnakeGame PRIVATE snake_controller)

    # Copy assets to build directory if present
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
        file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
    endif()

    # On Windows, copy SFML runtime DLLs to build output directory so all executables and tests run
    if(WIN32)
        file(GLOB SFML_RUNTIME_DLLS
            "${SFML_DIR}/lib/*.dll"
            "${SFML_DIR}/bin/*.dll"
        )
        if(SFML_RUNTIME_DLLS)
            file(COPY ${SFML_RUNTIME_DLLS} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
        endif()

        # If MinGW runtime DLLs exist, copy them so binaries run standalone
        if(MINGW)
            get_filename_component(MINGW_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
            file(GLOB MINGW_RUNTIME_DLLS
                "${MINGW_BIN_DIR}/*.dll"
                "${MINGW_BIN_DIR}/../x86_64-w64-mingw32/lib/gcc/x86_64-w64-mingw32/*/*.dll"
            )
            if(MINGW_RUNTIME_DLLS)
                file(COPY ${MINGW_RUNTIME_DLLS} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
            endif()
        endif()
    endif()
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

    # Collision unit tests
    add_executable(collision_tests tests/collision_detector_test.cpp)
    target_link_libraries(collision_tests PRIVATE snake_collision GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(collision_tests)

    if(BUILD_GAME)
        # Movement unit tests (Charan's original test target)
        add_executable(SnakeTests tests/snake_test.cpp)
        target_link_libraries(SnakeTests PRIVATE snake)
        add_test(NAME SnakeMovementTests COMMAND SnakeTests)

        # Full game integration tests
        add_executable(game_integration_tests tests/game_integration_test.cpp)
        target_link_libraries(game_integration_tests PRIVATE snake_controller GTest::gtest_main)
        gtest_discover_tests(game_integration_tests)
    endif()
endif()
