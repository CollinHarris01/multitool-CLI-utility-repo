#include "../include/stats_tool.h"

#include <iostream>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <cstdlib>

namespace fs = std::filesystem;

static int testsPassed = 0;
static int testsFailed = 0;

#define CHECK(expr) \
    do { \
        if (!(expr)) { \
            std::cerr << "CHECK FAILED: " #expr << std::endl; \
            ++testsFailed; \
        } else { \
            ++testsPassed; \
        } \
    } while (0)


// -----------------------------------------------------------------------------
// Capture stdout temporarily
// -----------------------------------------------------------------------------
class OutputCapture {
public:
    OutputCapture() {
        old = std::cout.rdbuf(buffer.rdbuf());
    }
    ~OutputCapture() {
        std::cout.rdbuf(old);
    }
    std::string str() const {
        return buffer.str();
    }
private:
    std::stringstream buffer;
    std::streambuf* old;
};


// -----------------------------------------------------------------------------
// Test running stats on a directory
// -----------------------------------------------------------------------------
void test_directory_stats() {
    std::cout << "[TEST] test_directory_stats\n";

    fs::path root = fs::temp_directory_path() / "stats_ctest_dir";
    fs::remove_all(root);
    fs::create_directories(root);

    // Make files
    fs::path file1 = root / "file1.txt";
    fs::path file2 = root / "file2.txt";

    {
        std::ofstream(file1) << "123456789012345678901234567890";      // 30 bytes
        std::ofstream(file2) << "123456789012345678901234567";         // 27 bytes
    }

    // Subdir
    fs::path sub = root / "subdir";
    fs::create_directories(sub);

    fs::path nested = sub / "nested.txt";
    {
        std::ofstream(nested) << "123456789012345678901234567890";     // 30 bytes
    }

    StatsCommand cmd;
    cmd.targetPath = root.string();

    OutputCapture capture;
    cmd.run();
    std::string out = capture.str();

    // Directory header
    CHECK(out.find("Directory: \"" + fs::absolute(root).string() + "\"") != std::string::npos);

    // Recursive totals
    CHECK(out.find("Total files (recursive): 3") != std::string::npos);
    CHECK(out.find("Total directories (recursive): 1") != std::string::npos);
    CHECK(out.find("Total size (recursive): 87.0 B") != std::string::npos);

    // File grouping under `.txt`
    CHECK(out.find("Files (immediate):") != std::string::npos);
    CHECK(out.find("  .txt") != std::string::npos);
    CHECK(out.find("    file1.txt — 30.0 B") != std::string::npos);
    CHECK(out.find("    file2.txt — 27.0 B") != std::string::npos);

    // Subdirectory headings
    CHECK(out.find("Subdirectories (immediate):") != std::string::npos);
    CHECK(out.find("  " + fs::absolute(sub).string()) != std::string::npos);
    CHECK(out.find("    Total size (recursive): 30.0 B") != std::string::npos);
    CHECK(out.find("    File count (recursive): 1") != std::string::npos);

    fs::remove_all(root);
}


// -----------------------------------------------------------------------------
// Test running stats on a single file
// -----------------------------------------------------------------------------
void test_file_stats() {
    std::cout << "[TEST] test_file_stats\n";

    fs::path file = fs::temp_directory_path() / "stats_ctest_file.txt";
    fs::remove(file);

    {
        std::ofstream(file) << "123456789012345678901234567890";       // 30 bytes
    }

    StatsCommand cmd;
    cmd.targetPath = file.string();

    OutputCapture capture;
    cmd.run();
    std::string out = capture.str();

    CHECK(out.find("File: \"" + fs::absolute(file).string() + "\"") != std::string::npos);
    CHECK(out.find("Type: .txt") != std::string::npos);
    CHECK(out.find("Size: 30.0 B") != std::string::npos);
    CHECK(out.find("Last modified:") != std::string::npos);

    fs::remove(file);
}


// -----------------------------------------------------------------------------
// main for ctest
// -----------------------------------------------------------------------------
int main() {
    test_directory_stats();
    test_file_stats();

    std::cout << "\nTests passed: " << testsPassed << "\n";
    std::cout << "Tests failed: " << testsFailed << "\n";

    return (testsFailed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}