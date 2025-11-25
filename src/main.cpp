#include "../include/search_tool.h"
#include "../include/stats_tool.h"
#include "../include/hash_tool.h"
#include "../include/copy_tool.h"

#include <filesystem>
#include <iostream>
#include <string>

#include <CLI/CLI.hpp>

// Register "search" CLI11 subcommand
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

// Register "stats" CLI11 subcommand
void registerStatsCommand(CLI::App& app) {
    static StatsCommand statsCmd;

    auto statsSub = app.add_subcommand("stats", "Show file/directory statistics");

    // Required positional arguments
    statsSub->add_option("path", statsCmd.targetPath, "File or directory to inspect")->required();

    // CLI11 callback calls run() on StatsCommand struct
    statsSub->callback([&]() { statsCmd.run(); });
}

// Register "hash" CLI11 sucommand
void registerHashCommand(CLI::App& app) {
    static HashCommand hashCmd;

    auto hashSub = app.add_subcommand("hash", "Compute SHA-256 hash values for files in a directory");

    // Require positional arguments
    hashSub->add_option("path", hashCmd.targetPath, "File or directory to hash")->required();

    // Optional flags
    hashSub->add_flag("-R,--recursive", hashCmd.recursive, "Enable recursive directory hashing");

    // CLI11 callback calls run() on HashCommand struct
    hashSub->callback([&]() { hashCmd.run(); });
}

// Register "copy" CLI11 sucommand
void registerCopyCommand(CLI::App& app) {
    static CopyCommand copyCmd;

    auto copySub = app.add_subcommand("copy", "Copy a file or directory to another location");

    // Required positional arguments
    copySub->add_option("source", copyCmd.sourcePath, "Source file/directory to copy")->required();
    copySub->add_option("destination", copyCmd.destinationPath, "Destination for copied file/directory")->required();

    // Optional flags
    copySub->add_flag("-R,--recursive", copyCmd.recursive, "Enable recursive directory copying");
    copySub->add_flag("--force", copyCmd.force, "Overwrite destination files if they already exist");

    // CLI11 callback calls run() on CopyCommand struct
    copySub->callback([&]() { copyCmd.run(); });
}

// ---- Main ----
int main(int argc, char** argv) {
    // Create CLI application
    CLI::App app{"Toolkit - Command Line Utilities"};

    // Register subcommands
    registerSearchCommand(app);
    registerStatsCommand(app);
    registerHashCommand(app);
    registerCopyCommand(app);

    // Parse CLI input
    CLI11_PARSE(app, argc, argv);
    
    // If no subcommand provided, print help
    if (argc == 1) {
        std::cout << app.help() << std::endl;
        return 0;
    }

    return 0;
}