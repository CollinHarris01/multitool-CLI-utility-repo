#include "../include/search_tool.h"
#include "../include/stats_tool.h"

#include <filesystem>
#include <iostream>
#include <string>

#include <CLI/CLI.hpp>

// Function to register "search" CLI11 subcommand
void registerSearchCommand(CLI::App& app) {
    static SearchCommand searchCmd;

    auto searchSub = app.add_subcommand("search", "Search for a pattern in files within a directory");

    // Required positional arguments
    searchSub->add_option("directory", searchCmd.directory, "Directory to search")->required();
    searchSub->add_option("pattern", searchCmd.pattern, "Pattern to search for")->required();

    // Optional flags
    searchSub->add_flag("--case", searchCmd.caseSensitive, "Enable case-sensitive matches");
    searchSub->add_flag("-R,--recursive", searchCmd.recursive, "Enable recursive directory search");
    searchSub->add_flag("-v,--verbose", searchCmd.verbose, "Print matches per file to terminal");

    // CLI11 callback calls run() on SearchCommand struct
    searchSub->callback([&]() { searchCmd.run(); });
}

// Function to register "stats" CLI11 subcommand
void registerStatsCommand(CLI::App& app) {
    static StatsCommand statsCmd;

    auto statsSub = app.add_subcommand("stats", "Show file/directory statistics");

    // Required positional arguments
    statsSub->add_option("path", statsCmd.targetPath, "File or directory to inspect")->required();

    // CLI11 callback calls run() on StatsCommand struct
    statsSub->callback([&]() { statsCmd.run(); });
}

// ---- Main ----
int main(int argc, char** argv) {
    // Create CLI application
    CLI::App app{"Toolkit - Command Line Utilities"};

    // Register subcommands
    registerSearchCommand(app);
    registerStatsCommand(app);

    // Parse CLI input
    CLI11_PARSE(app, argc, argv);
    
    // If no subcommand provided, print help
    if (argc == 1) {
        std::cout << app.help() << std::endl;
        return 0;
    }

    return 0;
}