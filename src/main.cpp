#include "../include/search_tool.h"
#include "../include/stats_tool.h"
#include "../include/hash_tool.h"
#include "../include/copy_tool.h"
#include "../include/move_tool.h"
#include "../include/remove_tool.h"
#include "../include/tree_tool.h"

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
    searchSub->add_flag("-r,--recursive", searchCmd.recursive, "Enable recursive directory search");
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
    hashSub->add_flag("-r,--recursive", hashCmd.recursive, "Enable recursive directory hashing");

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
    copySub->add_flag("-r,--recursive", copyCmd.recursive, "Enable recursive directory copying");
    copySub->add_flag("-f,--force", copyCmd.force, "Overwrite destination files if they already exist");

    // CLI11 callback calls run() on CopyCommand struct
    copySub->callback([&]() { copyCmd.run(); });
}

// Register "move" CLI11 command
void registerMoveCommand(CLI::App& app) {
    static MoveCommand moveCmd;

    auto moveSub = app.add_subcommand("move", "Move a file or directory");

    // Required positional arguments
    moveSub->add_option("source", moveCmd.sourcePath, "Source file/directory")->required();
    moveSub->add_option("destination", moveCmd.destinationPath, "Destination path")->required();

    // Optional flags
    moveSub->add_flag("-f,--force", moveCmd.force, "Overwrite if destination exists");
    moveSub->add_flag("-r,--recursive", moveCmd.recursive, "Allow recursive directory moving");

    // CLI11 callback calls run() on MoveCommand struct
    moveSub->callback([&]() { moveCmd.run(); });
}

// Register "remove" CLI11 subcommand
void registerRemoveCommand(CLI::App& app) {
    static RemoveCommand removeCmd;

    auto removeSub = app.add_subcommand("remove", "Delete a file or directory");

    // Requied positional arguments
    removeSub->add_option("path", removeCmd.targetPath, "Path to file or directory to delete")->required();

    // Optional flags
    removeSub->add_flag("-r,--recursive", removeCmd.recursive, "Recursively delete directories");
    removeSub->add_flag("-f,--force", removeCmd.force, "Delete without confirmation prompt");

    // CLI11 callback calls run() on RemoveCommand struct
    removeSub->callback([&]() { removeCmd.run(); });
}

// Register "tree" CLI11 subcommand
void registerTreeCommand(CLI::App& app) {
    static TreeCommand treeCmd;

    auto treeSub = app.add_subcommand("tree", "Display directory structure as an ASCII tree");

    // Required positional arguments
    treeSub->add_option("path", treeCmd.path, "Directory path to display")->required();

    // Optional flags
    treeSub->add_flag("--dirs-only", treeCmd.dirsOnly, "Display only directories");

    // CLI11 callback calls run() on TreeCommand struct
    treeSub->callback([&]() { treeCmd.run(); });
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
    registerMoveCommand(app);
    registerRemoveCommand(app);
    registerTreeCommand(app);

    // Parse CLI input
    CLI11_PARSE(app, argc, argv);
    
    // If no subcommand provided, print help
    if (argc == 1) {
        std::cout << app.help() << std::endl;
        return 0;
    }

    return 0;
}