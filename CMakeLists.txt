cmake_minimum_required(VERSION 3.20)
set(SNAKE_VERSION "1.0.0" CACHE STRING "Release version (overridden by the release workflow from the git tag)")
project(SnakeGame VERSION ${SNAKE_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------------------------
# Quality / DevSecOps options (used by CI; all OFF by default for local builds)
# ---------------------------------------------------------------------------
option(SNAKE_ENABLE_WARNINGS   "Compile project code with -Wall -Wextra -Wpedantic" OFF)
option(SNAKE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors (needs SNAKE_ENABLE_WARNINGS)" OFF)
option(SNAKE_ENABLE_SANITIZERS "Build with AddressSanitizer + UndefinedBehaviorSanitizer (TC-07, TC-13)" OFF)

# Applied per project target below so third-party code (GoogleTest) is untouched.
function(snake_apply_quality_flags target)
    if(SNAKE_ENABLE_WARNINGS)
        if(MSVC)
            target_compile_options(${target} PRIVATE /W4)
            if(SNAKE_WARNINGS_AS_ERRORS)
                target_compile_options(${target} PRIVATE /WX)
            endif()
        else()
            target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
            if(SNAKE_WARNINGS_AS_ERRORS)
                target_compile_options(${target} PRIVATE -Werror)
            endif()
        endif()
    endif()
endfunction()

if(SNAKE_ENABLE_SANITIZERS AND NOT MSVC)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all)
    add_link_options(-fsanitize=address,undefined)
endif()

# Link GCC runtime statically on MinGW to prevent missing runtime DLLs
if(MINGW)
    add_link_options(-static-libgcc -static-libstdc++)
endif()

# Core domain libraries (independent of graphical display)
add_library(snake_collision src/collision_detector.cpp)
target_include_directories(snake_collision PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(snake_food src/food_manager.cpp)
target_include_directories(snake_food PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(snake_score src/score_manager.cpp)
target_include_directories(snake_score PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

option(BUILD_GAME "Build the SFML game and movement module" ON)
if(BUILD_GAME)
    # Use SFML_DIR or CMAKE_PREFIX_PATH for non-standard installations.
    find_package(SFML 2 COMPONENTS graphics window system REQUIRED)

    add_library(snake src/snake.cpp)
    target_include_directories(snake PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
    target_link_libraries(snake PUBLIC sfml-graphics sfml-window sfml-system)

    add_library(snake_controller src/game_controller.cpp)
    target_include_directories(snake_controller PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
    target_link_libraries(snake_controller PUBLIC snake snake_food snake_score snake_collision)

    add_executable(SnakeGame src/main.cpp)
    target_link_libraries(SnakeGame PRIVATE snake_controller)

    if(APPLE)
        set_target_properties(SnakeGame PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_GUI_IDENTIFIER "edu.pes.snakegame"
            MACOSX_BUNDLE_BUNDLE_NAME "Snake Game"
        )
        set_source_files_properties(assets/arial.ttf PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources/assets"
        )
        target_sources(SnakeGame PRIVATE assets/arial.ttf)
    endif()

    foreach(t snake snake_controller SnakeGame)
        snake_apply_quality_flags(${t})
    endforeach()

    # Packaging (Continuous Delivery): `cmake --install` / `cpack` lay out
    # SnakeGame next to assets/ so the font lookup in main.cpp works unchanged.
    if(APPLE)
        install(TARGETS SnakeGame BUNDLE DESTINATION .)
    else()
        install(TARGETS SnakeGame RUNTIME DESTINATION .)
        install(DIRECTORY assets DESTINATION .)
    endif()
    install(FILES README.md DESTINATION .)

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

foreach(t snake_collision snake_food snake_score)
    snake_apply_quality_flags(${t})
endforeach()

set(CPACK_PACKAGE_NAME "SnakeGame")
set(CPACK_PACKAGE_VENDOR "SE_JACKFRUIT team, PES University")
set(CPACK_PACKAGE_VERSION "${SNAKE_VERSION}")
set(CPACK_GENERATOR "TGZ;ZIP")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY ON)
include(CPack)

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

    # Food placement and scoring unit tests (Vrunda's module, TC-03 / TC-04)
    add_executable(food_score_tests tests/food_score_test.cpp)
    target_link_libraries(food_score_tests PRIVATE snake_food snake_score GTest::gtest_main)
    gtest_discover_tests(food_score_tests)

    if(BUILD_GAME)
        # Movement unit tests (Charan's original test target)
        add_executable(SnakeTests tests/snake_test.cpp)
        target_link_libraries(SnakeTests PRIVATE snake)
        add_test(NAME SnakeMovementTests COMMAND SnakeTests)

        # Full game integration tests
        add_executable(game_integration_tests tests/game_integration_test.cpp)
        target_link_libraries(game_integration_tests PRIVATE snake_controller GTest::gtest_main)
        gtest_discover_tests(game_integration_tests)

        # Food, growth and scoring through the game controller (TC-03 / TC-04)
        add_executable(food_score_integration_tests tests/food_score_integration_test.cpp)
        target_link_libraries(food_score_integration_tests PRIVATE snake_controller GTest::gtest_main)
        gtest_discover_tests(food_score_integration_tests)
    endif()
endif()
