#ifndef TREE_TOOL_H
#define TREE_TOOL_H

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

struct TreeCommand {
    fs::path path;
    bool dirsOnly = false;

    void run() const;
};

// Tree tool helper functions
void printTreeRecursive( 
    const fs::path& path, 
    const std::string& prefix, 
    bool isLast, 
    bool dirsOnly
);

#endif