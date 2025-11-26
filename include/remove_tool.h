#ifndef REMOVE_TOOL_H
#define REMOVE_TOOL_H

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

struct RemoveCommand {
    std::string targetPath;
    bool recursive = false;
    bool force = false;

    void run() const;
};

// Remove tool helper functions
bool confirmPrompt(const std::string& message);
bool deleteFile(const fs::path& filePath, std::string& errorMessage);
bool deleteDirectoryRecursive(const fs::path& dirPath, std::string& errorMessage);

#endif