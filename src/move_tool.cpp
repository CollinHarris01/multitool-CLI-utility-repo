#include "../include/move_tool.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <system_error>

namespace fs = std::filesystem;

// Copy + delete fallback for recursive directory move
bool moveRecursive(
    const fs::path& source,
    const fs::path& destination,
    bool force,
    bool recursive,
    std::string& errorMessage
) {
    // If attempting to move a directory but recursive is not enabled
    if (!recursive && fs::is_directory(source)) {
        errorMessage = "Source is a directory but --recursive not specified.";
        return false;
    }

    // If destination exists
    if (fs::exists(destination)) {
        if (!force) {
            errorMessage = "Destination exists (use --force to overwrite).";
            return false;
        }

        // Remove destination
        std::error_code rm_ec;
        fs::remove_all(destination, rm_ec);
        if (rm_ec) {
            errorMessage = "Failed to delete existing destination: " + rm_ec.message();
            return false;
        }
    }

    // First copy
    std::error_code cp_ec;
    if (fs::is_directory(source)) {
        fs::copy(
            source,
            destination,
            fs::copy_options::recursive | fs::copy_options::overwrite_existing,
            cp_ec
        );
    } else {
        fs::copy_file(
            source,
            destination,
            force ? fs::copy_options::overwrite_existing : fs::copy_options::none,
            cp_ec
        );
    }

    if (cp_ec) {
        errorMessage = "Copy failed: " + cp_ec.message();
        return false;
    }

    // Then delete source
    std::error_code del_ec;
    fs::remove_all(source, del_ec);
    if (del_ec) {
        errorMessage = "Copied but failed to delete original: " + del_ec.message();
        return false;
    }

    return true;
}

// MoveCommand::run()
void MoveCommand::run() const {
    fs::path src(sourcePath);
    fs::path dst(destinationPath);

    if (!fs::exists(src)) {
        std::cerr << "Error: Source does not exist.\n";
        return;
    }

    // Try direct rename first
    std::error_code ec;
    fs::rename(src, dst, ec);

    if (!ec) {
        std::cout << "Moved \"" << src.string() << "\" → \"" << dst.string() << "\"\n";
        return;
    }

    // If rename failed, attempt fallback
    std::string errorMessage;
    if (!moveRecursive(src, dst, force, recursive, errorMessage)) {
        std::cerr << "Error: " << errorMessage << "\n";
        return;
    }

    std::cout << "Moved \"" << src.string() << "\" → \"" << dst.string() << "\" (fallback method)\n";
}