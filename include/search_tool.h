#ifndef SEARCH_TOOL_H
#define SEARCH_TOOL_H

#include <string>
#include <vector>

struct SearchCommand {
    std::string directory;
    std::string pattern;
    bool caseSensitive = false;
    bool recursive = false;
    bool verbose = false;

    void run() const;
};

struct FileMatch {
    std::string filepath;
    int matches;
};

 // Search a single file for a substring, returns number of matches found
int searchInFile(const std::string& filepath, 
                 const std::string& pattern, 
                 bool caseSensitive = false);

// Search all files in a directory for a substring, return total matches across all files
std::vector<FileMatch> searchInDirectory(const std::string& directory,
                                         const std::string& pattern,
                                         bool caseSensitive = false,
                                         bool recursive = false);

#endif