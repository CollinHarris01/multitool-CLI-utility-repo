
# ---- Define project name and specs ----
cmake_minimum_required(VERSION 3.22)
project(toolkit LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include CMake's FetchContent module to download and include external libraries
include(FetchContent)

# ---- Fetch dependencies ----

# CLI11
FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.4.1
)
FetchContent_MakeAvailable(CLI11)

# nlohmann JSON
FetchContent_Declare(
    JSON
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(JSON)

# spdlog
FetchContent_Declare(
    SPDLOG
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.13.0
)
FetchContent_MakeAvailable(SPDLOG)

# ---- Add executables ----
add_executable(toolkit src/main.cpp)

target_sources(toolkit PRIVATE
    src/search_tool.cpp
    src/stats_tool.cpp
    src/sha256.cpp
    src/hash_tool.cpp
    src/copy_tool.cpp
    src/move_tool.cpp
)

# ---- Link dependencies ----
target_link_libraries(toolkit PRIVATE
    CLI11::CLI11
    nlohmann_json::nlohmann_json
    spdlog::spdlog
)

# ---- Include directories ----
target_include_directories(toolkit PRIVATE include)

# ---- Tests ----
enable_testing()
add_subdirectory(tests)