#include "../include/search_tool.h"

#include <filesystem>
#include <iostream>
#include <string>

#include <CLI/CLI.hpp>

// Function to register "search" CLI11 subcommand
void registerSearchCommand(CLI::App& app) {
    auto searchCmd = app.add_subcommand("search", "Search for a pattern in files within a directory");

    static SearchCommand cmd;

    // Required positional arguments
    searchCmd->add_option("directory", cmd.directory, "Directory to search")->required();
    searchCmd->add_option("pattern", cmd.pattern, "Pattern to search for")->required();

    // Optional flags
    searchCmd->add_flag("--case", cmd.caseSensitive, "Enable case-sensitive matches");
    searchCmd->add_flag("-R,--recursive", cmd.recursive, "Enable recursive directory search");
    searchCmd->add_flag("-v,--verbose", cmd.verbose, "Print matches per file to terminal");

    // CLI11 callback calls run() on SearchCommand struct
    searchCmd->callback([&]() { cmd.run(); });
}

// ---- Main ----
int main(int argc, char** argv) {
    // Create CLI application
    CLI::App app{"Toolkit - Command Line Utilities"};

    // Register subcommands
    registerSearchCommand(app);

    // Parse CLI input
    CLI11_PARSE(app, argc, argv);
    
    // If no subcommand provided, print help
    if (argc == 1) {
        std::cout << app.help() << std::endl;
        return 0;
    }

    return 0;
}