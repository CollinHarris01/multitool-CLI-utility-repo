#include "../include/remove_tool.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <system_error>

namespace fs = std::filesystem;

// Prompt user for confirmation ("y/n"). Returns true if user confirms.
bool confirmPrompt(const std::string& message) {
    while (true) {
        std::cout << message << " (y/n): ";
        std::string input;
        if (!std::getline(std::cin, input)) {
            return false; // treat input error as "no"
        }

        if (input == "y" || input == "Y" || input == "yes" || input == "YES")
            return true;
        if (input == "n" || input == "N" || input == "no" || input == "NO")
            return false;

        std::cout << "Invalid response. Please enter y or n.\n";
    }
}

// Delete a single file. Returns true on success.
bool deleteFile(const fs::path& filePath, std::string& errorMessage) {
    std::error_code ec;
    if (!fs::remove(filePath, ec)) {
        errorMessage = ec ? ec.message() : "Failed to delete file";
        return false;
    }
    return true;
}

// Recursively delete a directory. Returns true on success.
bool deleteDirectoryRecursive(const fs::path& dirPath, std::string& errorMessage) {
    std::error_code ec;

    // remove_all returns count or -1 on error
    uintmax_t removed = fs::remove_all(dirPath, ec);
    if (ec || removed == static_cast<uintmax_t>(-1)) {
        errorMessage = ec ? ec.message() : "Failed to delete directory.";
        return false;
    }

    return true;
}

// RemoveCommand::run()
void RemoveCommand::run() const {
    fs::path target = targetPath;

    // Check existence
    if (!fs::exists(target)) {
        std::cerr << "Error: \"" << target.string() << "\" does not exist.\n";
        return;
    }

    bool isDir = fs::is_directory(target);

    // Directory deletion requires --recursive
    if (isDir && !recursive) {
        std::cerr << "Error: \"" << target.string()
                  << "\" is a directory. Use --recursive to delete directories.\n";
        return;
    }

    // Prompt unless forced
    if (!force) {
        std::string msg = "Are you sure you want to delete \"" + target.string() + "\"?";
        if (!confirmPrompt(msg)) {
            std::cout << "Deletion cancelled.\n";
            return;
        }
    }

    // Perform deletion
    std::string errorMessage;

    bool success = false;
    if (isDir) {
        success = deleteDirectoryRecursive(target, errorMessage);
    } else {
        success = deleteFile(target, errorMessage);
    }

    if (!success) {
        std::cerr << "Error deleting \"" << target.string()
                  << "\": " << errorMessage << "\n";
        return;
    }

    std::cout << "Deleted \"" << target.string() << "\"\n";
}