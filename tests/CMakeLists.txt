
# ---- Enable ctest ----
enable_testing()

# ---- Create executable ----
add_executable(unit_tests basic_test.cpp)


# ---- Link dependencies ----
target_link_libraries(unit_tests
    PRIVATE
        CLI11::CLI11
)

# ---- Register test w/ ctest ----
add_test(NAME BasicTests COMMAND unit_tests)