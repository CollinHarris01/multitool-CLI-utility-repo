#ifndef SEARCH_TOOL_H
#define SEARCH_TOOL_H

#include <string>
#include <vector>

class SearchTool {
public:
    // Search a single file for a substring, returns number of matches found
    int searchInFile(const std::string& filePath, 
                     const std::string& searchString, 
                     bool caseSensitive = true);

    // Search all files in a directory for a substring, return total matches across all files
    int searchInDirectory(const std::string& directoryPath,
                          const std::string& searchString,
                          bool caseSensitive = true);
};

#endif