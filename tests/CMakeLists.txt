
# ---- Enable ctest ----
enable_testing()

# ---- Create a library from search_tool for tests ----
add_library(search_tool_lib ../src/search_tool.cpp)
add_library(stats_tool_lib ../src/stats_tool.cpp)
add_library(hash_tool_lib ../src/hash_tool.cpp ../src/sha256.cpp)
add_library(copy_tool_lib ../src/copy_tool.cpp)
add_library(move_tool_lib ../src/move_tool.cpp)

# ---- Create test executables ----
add_executable(basic_test basic_test.cpp)
add_executable(search_test search_test.cpp)
add_executable(stats_test stats_test.cpp)
add_executable(hash_test hash_test.cpp)
add_executable(copy_test copy_test.cpp)
add_executable(move_test move_test.cpp)

# ---- Link dependencies ----
target_link_libraries(basic_test PRIVATE CLI11::CLI11)
target_link_libraries(search_test PRIVATE search_tool_lib)
target_link_libraries(stats_test PRIVATE stats_tool_lib)
target_link_libraries(hash_test PRIVATE hash_tool_lib)
target_link_libraries(copy_test PRIVATE copy_tool_lib)
target_link_libraries(move_test PRIVATE move_tool_lib)

# ---- Register test w/ ctest ----
add_test(NAME BasicTest COMMAND basic_test)
add_test(NAME SearchTest COMMAND search_test)
add_test(NAME StatsTest COMMAND stats_test)
add_test(NAME HashTest COMMAND hash_test)
add_test(NAME CopyTest COMMAND copy_test)
add_test(NAME MoveTest COMMAND move_test)

# ---- Aggregate all tests under a single target ----
add_custom_target(unit_tests
    COMMENT "Build all unit test executables"
    DEPENDS 
        basic_test 
        search_test 
        stats_test 
        hash_test
        copy_test
        move_test
)