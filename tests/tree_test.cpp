#include "../include/tree_tool.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace fs = std::filesystem;

std::string captureTreeOutput(TreeCommand& cmd) {
    std::ostringstream oss;
    auto cout_buf = std::cout.rdbuf(); // save original buffer
    std::cout.rdbuf(oss.rdbuf());      // redirect std::cout to oss

    cmd.run();

    std::cout.rdbuf(cout_buf);         // restore original buffer
    return oss.str();
}

int main() {
    // Setup temporary test directory
    fs::path tmpDir = fs::temp_directory_path() / "tree_tool_test";
    fs::remove_all(tmpDir); // ensure clean start
    fs::create_directories(tmpDir / "subdir");

    // Create files
    std::ofstream(tmpDir / "file1.txt") << "Hello";
    std::ofstream(tmpDir / "file2.log") << "World";
    std::ofstream(tmpDir / "subdir" / "file3.txt") << "Nested";

    // ---- Test 1: Full tree ----
    TreeCommand cmd1;
    cmd1.path = tmpDir.string();
    cmd1.dirsOnly = false;

    std::string output1 = captureTreeOutput(cmd1);

    assert(output1.find("file1.txt") != std::string::npos);
    assert(output1.find("file2.log") != std::string::npos);
    assert(output1.find("subdir") != std::string::npos);
    assert(output1.find("file3.txt") != std::string::npos);

    // ---- Test 2: Dirs only ----
    TreeCommand cmd2;
    cmd2.path = tmpDir.string();
    cmd2.dirsOnly = true;

    std::string output2 = captureTreeOutput(cmd2);

    assert(output2.find("file1.txt") == std::string::npos);
    assert(output2.find("file2.log") == std::string::npos);
    assert(output2.find("subdir") != std::string::npos);

    // Cleanup
    fs::remove_all(tmpDir);

    std::cout << "All TreeTool tests passed!" << std::endl;
    return 0;
}