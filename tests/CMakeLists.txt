
# ---- Enable ctest ----
enable_testing()

# ---- Create a library from search_tool for tests ----
add_library(search_tool_lib ../src/search_tool.cpp)
add_library(stats_tool_lib ../src/stats_tool.cpp)
add_library(hash_tool_lib ../src/hash_tool.cpp ../src/sha256.cpp)

# ---- Create test executables ----
add_executable(basic_test basic_test.cpp)
add_executable(search_test search_test.cpp)
add_executable(stats_test stats_test.cpp)
add_executable(hash_test hash_test.cpp)

# ---- Link dependencies ----
target_link_libraries(basic_test PRIVATE CLI11::CLI11)
target_link_libraries(search_test PRIVATE search_tool_lib)
target_link_libraries(stats_test PRIVATE stats_tool_lib)
target_link_libraries(hash_test PRIVATE hash_tool_lib)

# ---- Register test w/ ctest ----
add_test(NAME BasicTest COMMAND basic_test)
add_test(NAME SearchTest COMMAND search_test)
add_test(NAME StatsTest COMMAND stats_test)
add_test(NAME HashTest COMMAND hash_test)

# ---- Aggregate all tests under a single target ----
add_custom_target(unit_tests
    COMMENT "Build all unit test executables"
    DEPENDS basic_test search_test stats_test hash_test
)