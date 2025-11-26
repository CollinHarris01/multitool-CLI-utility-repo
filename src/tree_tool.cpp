#include "../include/tree_tool.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

// Print directory tree structure
void printTreeRecursive(
    const fs::path& path,
    const std::string& prefix,
    bool isLast,
    bool dirsOnly
) {
    std::vector<fs::directory_entry> entries;

    for (const auto& entry : fs::directory_iterator(path)) {
        if (dirsOnly && !entry.is_directory())
            continue;
        entries.push_back(entry);
    }

    for (size_t i = 0; i < entries.size(); ++i) {
        const bool last = (i == entries.size() - 1);
        const auto& entry = entries[i];

        std::cout << prefix
                  << (last ? "└── " : "├── ")
                  << entry.path().filename().string()
                  << "\n";

        if (entry.is_directory()) {
            std::string newPrefix = prefix + (last ? "    " : "│   ");
            printTreeRecursive(entry.path(), newPrefix, last, dirsOnly);
        }
    }
}

// TreeCommand::run()
void TreeCommand::run() const {
    try {
        if (!fs::exists(path)) {
            std::cerr << "Error: Path does not exist: " 
                      << path.string() << "\n";
            return;
        }

        // Print root name
        std::cout << path.filename().string() << "\n";

        // Begin recursive print
        printTreeRecursive(path, "", true, dirsOnly);
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }
}