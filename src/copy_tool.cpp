#include "../include/copy_tool.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

// Returns true if a filesystem path exists
bool pathExists(const fs::path& p) {
    std::error_code ec;
    return fs::exists(p, ec);
}

// Copies a single file, returns true on success
bool copySingleFile(const fs::path& src, const fs::path& dst, bool force) {
    std::error_code ec;

    if (pathExists(dst)) {
        if (!force) {
            std::cerr
                << "Error: \"" << dst.string()
                << "\" already exists (use --force to overwrite)\n";
            return false;
        }
        fs::remove(dst, ec);
        if (ec) {
            std::cerr << "Error: Could not overwrite \"" << dst.string()
                      << "\": " << ec.message() << "\n";
            return false;
        }
    }

    fs::copy_file(src, dst, fs::copy_options::overwrite_existing, ec);
    if (ec) {
        std::cerr << "Error copying \"" << src.string()
                  << "\" → \"" << dst.string()
                  << "\": " << ec.message() << "\n";
        return false;
    }

    std::cout << "Copied \"" << src.string()
              << "\" → \"" << dst.string() << "\"\n";
    return true;
}

// Handles recursive directory copy, returns true on success
bool copyDirectory(const fs::path& src, const fs::path& dst, bool force, bool recursive) {
    // Check recursion requirement
    if (!recursive) {
        std::cerr
            << "Error: Source is a directory but --recursive not specified.\n";
        return false;
    }

    std::error_code ec;

    // Create destination directory if needed
    if (!pathExists(dst)) {
        fs::create_directories(dst, ec);
        if (ec) {
            std::cerr << "Error creating directory \""
                      << dst.string() << "\": " << ec.message() << "\n";
            return false;
        }
    }

    // Iterate through directory contents
    for (const auto& entry : fs::directory_iterator(src)) {
        fs::path from = entry.path();
        fs::path to = dst / from.filename();

        if (entry.is_directory()) {
            // Recursive copy into subdirectory
            if (!copyDirectory(from, to, force, recursive)) {
                return false;
            }
        }
        else if (entry.is_regular_file()) {
            if (!copySingleFile(from, to, force)) {
                return false;
            }
        }
        else {
            std::cerr << "Skipping non-regular file: " << from.string() << "\n";
        }
    }

    return true;
}

// CopyCommand::run()
void CopyCommand::run() const {
    fs::path src(sourcePath);
    fs::path dst(destinationPath);

    std::error_code ec;

    // Validate source exists
    if (!fs::exists(src, ec)) {
        std::cerr << "Error: Source path \"" << sourcePath
                  << "\" does not exist.\n";
        return;
    }

    // If source is a file
    if (fs::is_regular_file(src, ec)) {
        // If destination is an existing directory, copy inside it (like cp behavior)
        if (fs::is_directory(dst, ec)) {
            dst /= src.filename();
        }

        copySingleFile(src, dst, force);
        return;
    }

    // If source is a directory, handle accordingly
    if (fs::is_directory(src, ec)) {
        if (!recursive) {
            std::cerr
                << "Error: Source is a directory but --recursive not specified.\n";
            return;
        }

        if (!copyDirectory(src, dst, force, recursive)) {
            std::cerr << "Directory copy failed.\n";
        }
        return;
    }

    // Unknown file type
    std::cerr << "Error: Unsupported source file type: " << sourcePath << "\n";
}