#include <CLI/CLI.hpp>
#include <iostream>

int main(int argc, char** argv) {
    CLI::App app{"System Diagnostics Tool"};

    int verbosity = 0;
    app.add_option("-v,--verbosity", verbosity, "Verbosity level");

    try {
        app.parse(argc, argv);
    }
    catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    std::cout << "Diagnostics tool starting...\n";
    std::cout << "Verbosity = " << verbosity << "\n";

    return 0;
}