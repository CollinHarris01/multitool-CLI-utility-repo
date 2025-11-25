#ifndef HASH_TOOL_H
#define HASH_TOOL_H

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

struct HashCommand {
    std::string targetPath;
    bool recursive;

    void run() const;
};

// Hash tool helper functions
std::string hashFile(const fs::path& filePath, bool& success);
void printDirectoryHashes(const fs::path& dirPath, bool recursive, int indentLevel);

#endif