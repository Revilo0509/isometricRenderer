cmake_minimum_required(VERSION 3.15)
project(isometricRenderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Source Files ---
file(GLOB SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# --- External Dependencies ---
include(FetchContent)

# raylib
find_package(raylib CONFIG)
if (NOT raylib_FOUND)
    message(STATUS "raylib not found. Fetching via FetchContent.")
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
    )
    FetchContent_MakeAvailable(raylib)
endif()

# raygui
find_package(raygui CONFIG)
if (NOT raygui_FOUND)
    message(STATUS "raygui not found. Fetching via FetchContent.")
    FetchContent_Declare(
        raygui
        GIT_REPOSITORY https://github.com/raysan5/raygui.git
        GIT_TAG 4.0
    )
    FetchContent_MakeAvailable(raygui)
endif()

# --- Executable ---
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
    "${raygui_SOURCE_DIR}/src"  # Or just ${raygui_SOURCE_DIR} depending on where raygui.h is
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    raylib
)
