#include "../include/copy_tool.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

static bool createFile(const fs::path &path, const std::string &contents) {
    std::ofstream ofs(path);
    if (!ofs) return false;
    ofs << contents;
    return true;
}

static bool fileContentsMatch(const fs::path &path, const std::string &contents) {
    std::ifstream ifs(path);
    if (!ifs) return false;
    std::string read;
    std::getline(ifs, read);
    return read == contents;
}

static void cleanup(const fs::path &path) {
    if (fs::exists(path)) {
        fs::remove_all(path);
    }
}

int main() {
    fs::path tempRoot = fs::temp_directory_path() / "copy_tool_test";
    fs::path sourceFile = tempRoot / "src_file.txt";
    fs::path destFile = tempRoot / "dest_file.txt";
    fs::path sourceDir = tempRoot / "src_dir";
    fs::path destDir = tempRoot / "dest_dir";

    cleanup(tempRoot);
    fs::create_directory(tempRoot);

    bool allPassed = true;

    // ==============================================================
    // TEST 1: Copy a single file
    // ==============================================================
    {
        std::cout << "TEST 1: Copying a single file... ";

        const std::string contents = "Hello, world!";
        fs::create_directories(tempRoot);
        if (!createFile(sourceFile, contents)) {
            std::cerr << "\nFailed to create source test file.\n";
            return 1;
        }

        CopyCommand cmd;
        cmd.sourcePath = sourceFile.string();
        cmd.destinationPath = destFile.string();
        cmd.force = false;

        cmd.run();

        if (!fs::exists(destFile) || !fileContentsMatch(destFile, contents)) {
            std::cerr << "\nCopy test failed.\n";
            allPassed = false;
        } else {
            std::cout << "OK\n";
        }

        cleanup(destFile);
        cleanup(sourceFile);
    }

    // ==============================================================
    // TEST 2: Do not overwrite without --force
    // ==============================================================
    {
        std::cout << "TEST 2: Refusing overwrite without --force... ";

        std::string original = "Original";
        std::string incoming = "Incoming";

        createFile(sourceFile, incoming);
        createFile(destFile, original);

        CopyCommand cmd;
        cmd.sourcePath = sourceFile.string();
        cmd.destinationPath = destFile.string();
        cmd.force = false;

        cmd.run();

        // Destination should remain unchanged
        if (!fileContentsMatch(destFile, original)) {
            std::cerr << "\nOverwrite copy without force failed.\n";
            allPassed = false;
        } else {
            std::cout << "OK\n";
        }

        cleanup(sourceFile);
        cleanup(destFile);
    }

    // ==============================================================
    // TEST 3: Overwrite WITH force
    // ==============================================================
    {
        std::cout << "TEST 3: Overwriting with --force... ";

        std::string original = "Original";
        std::string incoming = "Incoming";

        createFile(sourceFile, incoming);
        createFile(destFile, original);

        CopyCommand cmd;
        cmd.sourcePath = sourceFile.string();
        cmd.destinationPath = destFile.string();
        cmd.force = true;

        cmd.run();

        if (!fileContentsMatch(destFile, incoming)) {
            std::cerr << "\nForce overwrite failed.\n";
            allPassed = false;
        } else {
            std::cout << "OK\n";
        }

        cleanup(destFile);
        cleanup(sourceFile);
    }

    // ==============================================================
    // TEST 4: Copy a directory recursively
    // ==============================================================
    {
        std::cout << "TEST 4: Copying directory... ";

        fs::create_directory(sourceDir);
        createFile(sourceDir / "a.txt", "A");
        createFile(sourceDir / "b.txt", "B");

        CopyCommand cmd;
        cmd.sourcePath = sourceDir.string();
        cmd.destinationPath = destDir.string();
        cmd.force = false;
        cmd.recursive = true;

        cmd.run();

        bool pass =
            fs::exists(destDir / "a.txt") &&
            fs::exists(destDir / "b.txt") &&
            fileContentsMatch(destDir / "a.txt", "A") &&
            fileContentsMatch(destDir / "b.txt", "B");

        if (!pass) {
            std::cerr << "\nRecursive directory copy failed.\n";
            allPassed = false;
        } else {
            std::cout << "OK\n";
        }

        cleanup(destDir);
        cleanup(sourceDir);
    }

    cleanup(tempRoot);
    return allPassed ? 0 : 1;
}