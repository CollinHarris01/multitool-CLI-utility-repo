#include "../include/search_tool.h"

#include <cassert>
#include <fstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    // Use a temporary directory inside the current working directory
    fs::path testDir = fs::current_path() / "tmp_dir";
    fs::create_directories(testDir);  // creates any missing parent dirs

    // Create first test file
    std::ofstream file1(testDir / "file1.txt");
    file1 << "hello world\n";
    file1 << "this is a test\n";
    file1.close();

    // Create second test file
    std::ofstream file2(testDir / "file2.txt");
    file2 << "another line\n";
    file2 << "hello again\n";
    file2.close();

    SearchTool tool;

    // Test searchInFile
    int matchesFile1 = tool.searchInFile((testDir / "file1.txt").string(), "hello");
    assert(matchesFile1 == 1);  // file1 contains "hello" once

    int matchesFile2 = tool.searchInFile((testDir / "file2.txt").string(), "hello");
    assert(matchesFile2 == 1);  // file2 contains "hello" once

    // Test searchInDirectory (non-recursive)
    int totalMatches = tool.searchInDirectory(testDir.string(), "hello");
    assert(totalMatches == 2);  // both files together contain "hello" twice

    std::cout << "All search tests passed!\n";

    // Cleanup temporary files and directory
    fs::remove(testDir / "file1.txt");
    fs::remove(testDir / "file2.txt");
    fs::remove(testDir);

    return 0;
}