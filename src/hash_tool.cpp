#include "../include/hash_tool.h"
#include "../include/sha256.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// Compute SHA-256 hash of a file using your SHA256 class
std::string hashFile(const fs::path& filePath, bool& success) {
    success = false;

    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        return "<ERROR: unable to open file>";
    }

    SHA256 sha;
    std::vector<uint8_t> buffer(4096);

    while (!file.eof()) {
        file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
        std::streamsize bytesRead = file.gcount();
        if (bytesRead > 0) {
            sha.update(buffer.data(), static_cast<size_t>(bytesRead));
        }
    }

    sha.finalize();
    success = true;
    return sha.hexDigest();
}

// Helper to indent nicely
static void printIndent(int indent) {
    for (int i = 0; i < indent; i++) std::cout << "  ";
}

// Print hashes of files grouped by extension and recurse into subdirs
void printDirectoryHashes(const fs::path& dirPath, bool recursive, int indentLevel) {
    std::map<std::string, std::vector<fs::path>> filesByExt;
    std::vector<fs::path> subdirs;

    // Collect entries
    for (const auto& entry : fs::directory_iterator(dirPath)) {
        if (entry.is_directory()) {
            subdirs.push_back(entry.path());
        } else if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            if (ext.empty()) ext = "<no_ext>";
            filesByExt[ext].push_back(entry.path());
        }
    }

    // Print files grouped by extension
    for (const auto& [ext, fileList] : filesByExt) {
        printIndent(indentLevel);
        std::cout << ext << "\n";

        for (const auto& file : fileList) {
            bool ok;
            std::string hash = hashFile(file, ok);

            printIndent(indentLevel + 1);
            std::cout << file.filename().string() << " — ";

            if (ok)
                std::cout << hash << "\n";
            else
                std::cout << "<READ ERROR>\n";
        }
    }

    // Recurse into subdirectories
    if (recursive) {
        for (const auto& subdir : subdirs) {
            printIndent(indentLevel);
            std::cout << subdir.filename().string() << "/\n";
            printDirectoryHashes(subdir, true, indentLevel + 1);
        }
    }
}

// HashCommand::run()
void HashCommand::run() const {
    namespace fs = std::filesystem;

    fs::path path(targetPath);

    // Does the path exist?
    if (!fs::exists(path)) {
        std::cerr << "ERROR: Path does not exist: " << targetPath << "\n";
        return;
    }

    // Single file
    if (fs::is_regular_file(path)) {
        bool ok;
        std::string hash = hashFile(path, ok);

        std::cout << "File: " << fs::absolute(path).string() << "\n";
        if (ok)
            std::cout << "SHA-256: " << hash << "\n";
        else
            std::cout << "SHA-256: <READ ERROR>\n";

        return;
    }

    // Directory listing
    if (fs::is_directory(path)) {
        std::cout << "Directory: " << fs::absolute(path).string() << "\n";
        printDirectoryHashes(path, recursive, 1);
        return;
    }

    std::cerr << "ERROR: Path is not a file or directory: " << targetPath << "\n";
}