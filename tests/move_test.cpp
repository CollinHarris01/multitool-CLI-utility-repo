#include "../include/move_tool.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <cassert>

namespace fs = std::filesystem;

static void createFile(const fs::path& p, const std::string& text = "test") {
    std::ofstream ofs(p);
    ofs << text;
}

// -----------------------------------------------------------
// Test 1: Move a single file
// -----------------------------------------------------------
static void test_move_single_file() {
    std::cout << "[TEST] Move single file\n";

    fs::path base = fs::temp_directory_path() / "move_test_single";
    fs::path source = base / "source.txt";
    fs::path dest   = base / "moved.txt";

    fs::remove_all(base);
    fs::create_directories(base);

    createFile(source, "hello");

    std::string err;
    bool result = moveRecursive(source, dest, true, false, err);

    assert(result == true);
    assert(err.empty());
    assert(!fs::exists(source));
    assert(fs::exists(dest));

    fs::remove_all(base);
}

// -----------------------------------------------------------
// Test 2: Move directory recursively
// -----------------------------------------------------------
static void test_move_directory_recursive() {
    std::cout << "[TEST] Move directory recursively\n";

    fs::path base = fs::temp_directory_path() / "move_test_recursive";
    fs::path srcDir = base / "source";
    fs::path dstDir = base / "dest";

    fs::remove_all(base);
    fs::create_directories(srcDir);

    fs::create_directories(srcDir / "sub");
    createFile(srcDir / "file1.txt");
    createFile(srcDir / "sub" / "file2.txt");

    std::string err;
    bool result = moveRecursive(srcDir, dstDir, true, true, err);

    assert(result == true);
    assert(err.empty());
    assert(!fs::exists(srcDir));
    assert(fs::exists(dstDir / "file1.txt"));
    assert(fs::exists(dstDir / "sub" / "file2.txt"));

    fs::remove_all(base);
}

// -----------------------------------------------------------
// Test 3: Missing source
// -----------------------------------------------------------
static void test_missing_source_error() {
    std::cout << "[TEST] Missing source\n";

    fs::path base = fs::temp_directory_path() / "move_test_missing";
    fs::remove_all(base);
    fs::create_directories(base);

    fs::path source = base / "does_not_exist";
    fs::path dest   = base / "dest";

    std::string err;
    bool result = moveRecursive(source, dest, true, true, err);

    assert(result == false);
    assert(!err.empty());

    fs::remove_all(base);
}

// -----------------------------------------------------------
// Test 4: Force overwrite
// -----------------------------------------------------------
static void test_force_overwrite() {
    std::cout << "[TEST] Force overwrite\n";

    fs::path base = fs::temp_directory_path() / "move_test_overwrite";
    fs::path source = base / "source.txt";
    fs::path dest   = base / "dest.txt";

    fs::remove_all(base);
    fs::create_directories(base);

    createFile(source, "correct");
    createFile(dest, "old");

    std::string err;
    bool result = moveRecursive(source, dest, true, false, err);

    assert(result == true);
    assert(err.empty());
    assert(!fs::exists(source));
    assert(fs::exists(dest));

    // Validate content overwrite
    std::ifstream ifs(dest);
    std::string contents;
    std::getline(ifs, contents);
    assert(contents == "correct");

    fs::remove_all(base);
}

// ===========================================================
// main()
// ===========================================================
int main() {
    try {
        test_move_single_file();
        test_move_directory_recursive();
        test_missing_source_error();
        test_force_overwrite();
    } catch (const std::exception& e) {
        std::cerr << "[FAIL] Exception: " << e.what() << "\n";
        return 1;
    }

    std::cout << "\nAll move tests passed.\n";
    return 0;
}