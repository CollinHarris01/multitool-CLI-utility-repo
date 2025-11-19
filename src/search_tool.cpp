#include "../include/search_tool.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>

int SearchTool::searchInFile(const std::string& filePath, const std::string& searchString, bool caseSensitive) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filePath << "\n";
        return 0;
    }

    int matchCount = 0;
    std::string line;
    while (std::getline(file, line)) {
        std::string lineToSearch = line;
        std::string target = searchString;

        if (!caseSensitive) {
            std::transform(lineToSearch.begin(), lineToSearch.end(), lineToSearch.begin(), ::tolower);
            std::transform(target.begin(), target.end(), target.begin(), ::tolower);
        }

        if (lineToSearch.find(target) != std::string::npos) {
            std::cout << filePath << ": " << line << "\n";
            matchCount++;
        }
    }

    return matchCount;
}

int SearchTool::searchInDirectory(const std::string& directoryPath, const std::string& searchString, bool caseSensitive) {
    int totalMatches = 0;
    for (auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
            totalMatches += searchInFile(entry.path().string(), searchString, caseSensitive);
        }
    }
    return totalMatches;
}