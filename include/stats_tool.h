#ifndef STATS_TOOL_H
#define STATS_TOOL_H

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

struct StatsCommand {
    std::string targetPath;

    void run() const;
};

struct FileSummary {
    std::string filename;
    std::string extension;
    std::uintmax_t size;
};

struct SubdirSummary {
    std::string dirpath;
    std::uintmax_t totalSize;
    std::size_t fileCount;
};

struct DirectoryTotals {
    std::size_t totalFiles;
    std::size_t totalDirs;
    std::uintmax_t totalSize;
};

// Stats Tool helper functions
DirectoryTotals computeDirectoryTotals(const std::string& directory);
std::vector<FileSummary> listImmediateFiles(const std::string& directory);
std::vector<SubdirSummary> listImmediateSubdirs(const std::string& directory);
std::string formatSize(std::uintmax_t bytes);
std::string formatFileTime(const std::filesystem::file_time_type& ft);

#endif