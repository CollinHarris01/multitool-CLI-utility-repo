#ifndef COPY_TOOL_H
#define COPY_TOOL_H

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

struct CopyCommand {
    std::string sourcePath;
    std::string destinationPath;
    bool recursive = false;
    bool force = false;

    void run() const;
};

// Copy tool helper functions
bool pathExists(const fs::path& p);
bool copySingleFile(const fs::path& src, const fs::path& dst, bool force);
bool copyDirectory(const fs::path& src, const fs::path& dst, bool force, bool recursive);

#endif