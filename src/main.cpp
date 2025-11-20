#include "../include/search_tool.h"

#include <filesystem>
#include <iostream>
#include <string>

#include <CLI/CLI.hpp>

// ---- Search Command Struct ----
struct SearchCommand {
    std::string directory;
    std::string pattern;
    bool caseSensitive = false;
    bool recursive = false;
    bool verbose = false;

    void run() const {
        if (!std::filesystem::exists(directory)) {
            std::cerr << "Error -> Directory not found: " << directory << std::endl;
            std::exit(1);
        }

        auto results = searchInDirectory(directory, pattern, caseSensitive, recursive);

        if (verbose) {
            for (const auto& match : results) {
                std::cout << match.filepath << ": " << match.matches << " match"
                          << (match.matches > 1 ? "es" : "") << std::endl;
            }
        } else {
            int total = 0;
            for (const auto& match : results) total += match.matches;
            std::cout << "Matches found: " << total << std::endl;
        }
    }
};

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
    searchCmd->add_flag("-v,--verbose", cmd.verbose, "Print matches to stdout");

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