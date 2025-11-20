#include "../include/search_tool.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>

// Helper: Convert string to lowercase
std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

// Search all occurences in a single file
int searchInFile(const std::string& filepath, const std::string& pattern, bool caseSensitive) {
    std::ifstream file(filepath);
    if (!file.is_open()) return 0;

    int matchCount = 0;
    std::string line;
    std::string searchPattern = caseSensitive ? pattern : toLower(pattern);

    while (std::getline(file, line)) {
        std::string lineToSearch = caseSensitive ? line : toLower(line);

        // Count all occurences in the line
        size_t pos = 0;
        while ((pos = lineToSearch.find(searchPattern, pos)) != std::string::npos) {
            matchCount++;
            pos += searchPattern.length();
        }
    }

    return matchCount;
}

std::vector<FileMatch> searchInDirectory(const std::string& directory,
                                         const std::string& pattern,
                                         bool caseSensitive,
                                         bool recursive) {
    std::vector<FileMatch> results;

    try {
        std::filesystem::directory_options options = std::filesystem::directory_options::skip_permission_denied;

        if (recursive) {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(directory, options)) {
                if (entry.is_regular_file()) {
                    int matches = searchInFile(entry.path().string(), pattern, caseSensitive);
                    if (matches > 0)
                        results.push_back({entry.path().string(), matches});
                }
            }
        } else {
            for (const auto& entry : std::filesystem::directory_iterator(directory, options)) {
                if (entry.is_regular_file()) {
                    int matches = searchInFile(entry.path().string(), pattern, caseSensitive);
                    if (matches > 0)
                        results.push_back({entry.path().string(), matches});
                }
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }

    return results;
}