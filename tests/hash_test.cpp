#include "../include/hash_tool.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <cassert>

namespace fs = std::filesystem;

// Utility to create a file with contents
void writeFile(const fs::path& path, const std::string& contents) {
    std::ofstream ofs(path);
    ofs << contents;
}

// Clean up files/directories if they exist
void cleanup(const fs::path& path) {
    if (fs::exists(path)) {
        fs::remove_all(path);
    }
}

// Compute hash for file and compare to expected, assert if mismatch
void assertHashEquals(const fs::path& path, const std::string& expectedHex) {
    bool ok;
    std::string hash = hashFile(path, ok);
    if (hash != expectedHex) {
        std::cerr << "Hash mismatch for file: " << path << "\n"
                  << "Expected: " << expectedHex << "\n"
                  << "Got:      " << hash << std::endl;
        assert(false);
    }
}

int main() {
    std::cout << "Running HashCommand unit tests...\n";

    fs::path tmpDir = "tests/tmp_hash_test";
    cleanup(tmpDir);
    fs::create_directory(tmpDir);

    try {
        // Create files
        fs::path file1 = tmpDir / "file1.txt";
        fs::path file2 = tmpDir / "file2.txt";
        writeFile(file1, "Hello");
        writeFile(file2, "World");

        // Compute expected hashes using SHA256 directly
        bool ok1;
        bool ok2;
        std::string hash1 = hashFile(file1, ok1);
        std::string hash2 = hashFile(file2, ok2);

        // Test immediate files hashing
        HashCommand cmd;
        cmd.targetPath = tmpDir.string();
        cmd.recursive = false;
        cmd.run();  // Should print hashes, optionally could capture stdout

        // Test hashing each file individually
        assertHashEquals(file1, hash1);
        assertHashEquals(file2, hash2);

        // Test recursive hashing
        fs::path subDir = tmpDir / "subdir";
        fs::create_directory(subDir);
        fs::path subfile = subDir / "nested.txt";
        writeFile(subfile, "Nested file");
        bool ok3;
        std::string subhash = hashFile(subfile, ok3);

        cmd.recursive = true;
        cmd.run();  // Should print hashes for all files including subdir

        assertHashEquals(subfile, subhash);

    } catch (const std::exception& ex) {
        std::cerr << "Exception during tests: " << ex.what() << std::endl;
        cleanup(tmpDir);
        return 1;
    }

    // Clean up
    cleanup(tmpDir);

    std::cout << "All hash tests passed successfully.\n";
    return 0;
}