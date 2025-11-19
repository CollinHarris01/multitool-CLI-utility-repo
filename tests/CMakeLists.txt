
# ---- Enable ctest ----
enable_testing()

# ---- Create a library from search_tool for tests ----
add_library(search_tool_lib ../src/search_tool.cpp)

# ---- Create test executables ----
add_executable(basic_test basic_test.cpp)
add_executable(search_test search_test.cpp)


# ---- Link dependencies ----
target_link_libraries(basic_test
    PRIVATE
        CLI11::CLI11
)

target_link_libraries(search_test
    PRIVATE
        search_tool_lib
)

# ---- Register test w/ ctest ----
add_test(NAME BasicTest COMMAND basic_test)
add_test(NAME SearchTest COMMAND search_test)

# ---- Aggregate all tests under a single target ----
add_custom_target(unit_tests
    COMMENT "Build all unit test executables"
    DEPENDS basic_test search_test
)