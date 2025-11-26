#include "../include/remove_tool.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

static bool createFile(const fs::path& p, const std::string& contents) {
    std::ofstream ofs(p);
    if (!ofs) return false;
    ofs << contents;
    return true;
}

int main() {
    std::cout << "Running RemoveTest...\n";

    fs::path tmpDir = fs::temp_directory_path() / "remove_tool_test";

    // Clean up before starting
    std::error_code ec;
    fs::remove_all(tmpDir, ec);
    fs::create_directories(tmpDir);

    // -----------------------------------------
    // TEST 1: Remove a single file
    // -----------------------------------------

    {
        fs::path file = tmpDir / "test_file.txt";
        if (!createFile(file, "sample data")) {
            std::cout << "FAILED: Could not create test file\n";
            return 1;
        }

        if (!fs::exists(file)) {
            std::cout << "FAILED: Test file does not exist\n";
            return 1;
        }

        RemoveCommand cmd;
        cmd.targetPath = file.string();
        cmd.force = true;     // skip prompt
        cmd.recursive = false;

        cmd.run();

        if (fs::exists(file)) {
            std::cout << "FAILED: File was not deleted\n";
            return 1;
        }

        std::cout << "Test 1 passed: File removed successfully\n";
    }

    // -----------------------------------------
    // TEST 2: Remove a directory recursively
    // -----------------------------------------

    {
        fs::path dir = tmpDir / "test_dir";
        fs::path fileA = dir / "a.txt";
        fs::path fileB = dir / "b.txt";

        fs::create_directories(dir);
        createFile(fileA, "AAA");
        createFile(fileB, "BBB");

        if (!fs::exists(fileA) || !fs::exists(fileB)) {
            std::cout << "FAILED: Could not create directory contents\n";
            return 1;
        }

        RemoveCommand cmd;
        cmd.targetPath = dir.string();
        cmd.force = true;
        cmd.recursive = true;

        cmd.run();

        if (fs::exists(dir)) {
            std::cout << "FAILED: Directory was not deleted recursively\n";
            return 1;
        }

        std::cout << "Test 2 passed: Recursive directory delete successful\n";
    }

    // -----------------------------------------
    // Cleanup test root directory
    // -----------------------------------------

    fs::remove_all(tmpDir, ec);

    std::cout << "RemoveTest completed successfully.\n";
    return 0;
}