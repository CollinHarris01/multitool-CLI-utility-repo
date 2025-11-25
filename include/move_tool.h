#ifndef MOVE_TOOL_H
#define MOVE_TOOL_H

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

struct MoveCommand {
    std::string sourcePath;
    std::string destinationPath;
    bool force = false;
    bool recursive = false;

    void run() const;
};

// Attempt rename; if rename isn't possible (e.g. cross-device), fallback
bool moveRecursive(
    const fs::path& source, 
    const fs::path& destination, 
    bool force, 
    bool recursive,
    std::string& errorMessage
);

#endif