#include <CLI/CLI.hpp>
#include <cassert>
#include <vector>

int main() {
     CLI::App app{"Test"};

    int value = 0;
    app.add_option("-v,--value", value, "A test value");

    // Simulate command line: test_program --value 5
    std::vector<const char*> argv = {"test_program", "--value", "5"};
    int argc = static_cast<int>(argv.size());

    // Parse arguments
    app.parse(argc, const_cast<char**>(argv.data()));

    // Test result
    assert(value == 5);

    return 0;
}