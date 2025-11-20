#include "../include/search_tool.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

// Helper: Create a temporary test file with given content
void createFile(const fs::path& path, const std::string& content) {
    std::ofstream file(path);
    file << content;
}

// Helper: Remove a directory recursively
void removeDir(const fs::path& path) {
    fs::remove_all(path);
}

int main() {
    // ---- Setup test environment ----
    fs::path tmpDir = "tests/tmp_test_dir";
    fs::create_directories(tmpDir);

    fs::path file1 = tmpDir / "file1.txt";
    fs::path file2 = tmpDir / "file2.txt";
    fs::path subDir = tmpDir / "subdir";
    fs::create_directory(subDir);
    fs::path file3 = subDir / "file3.txt";

    createFile(file1, "hello world\nHELLO\nhello again");
    createFile(file2, "nothing here\nstill nothing");
    createFile(file3, "hello world\nhello\nhello hello");

    // ---- Test 1: searchInFile ----
    int count1 = searchInFile(file1.string(), "hello", true);  // case-sensitive
    assert(count1 == 2);  // "hello" appears twice

    int count2 = searchInFile(file1.string(), "HELLO", true);
    assert(count2 == 1);  // "HELLO" appears once

    int count3 = searchInFile(file1.string(), "hello", false); // case-insensitive
    assert(count3 == 3);  // all "hello" or "HELLO"

    // ---- Test 2: searchInDirectory (non-recursive) ----
    auto results1 = searchInDirectory(tmpDir.string(), "hello", false, false);
    assert(results1.size() == 1);               // only file1 contains matches
    assert(results1[0].filepath == file1.string());
    assert(results1[0].matches == 3);

    // ---- Test 3: searchInDirectory (recursive) ----
    auto results2 = searchInDirectory(tmpDir.string(), "hello", false, true);
    assert(results2.size() == 2);               // file1 and file3
    for (const auto& match : results2) {
        if (match.filepath == file1.string()) assert(match.matches == 3);
        else if (match.filepath == file3.string()) assert(match.matches == 4);
        else assert(false); // unexpected file
    }

    // ---- Test 4: case-sensitive recursive search ----
    auto results3 = searchInDirectory(tmpDir.string(), "HELLO", true, true);
    assert(results3.size() == 1);
    assert(results3[0].filepath == file1.string());
    assert(results3[0].matches == 1);

    // ---- Cleanup ----
    removeDir(tmpDir);

    std::cout << "All search tests passed!" << std::endl;
    return 0;
}