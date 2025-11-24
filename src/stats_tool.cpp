#include "../include/stats_tool.h"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;

// Get the size of files and directories (human-readable)
std::string formatSize(std::uintmax_t bytes) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    double size = static_cast<double>(bytes);
    int unit = 0;
    while (size >= 1024.0 && unit < 4) {
        size /= 1024.0;
        ++unit;
    }
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(size < 10.0 ? 2 : 1) << size << ' ' << units[unit];
    return oss.str();
}

// Convert filesystem::file_time_type to readable string (last_write_time)
std::string formatFileTime(const fs::file_time_type& ft) {
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>
            (ft - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
    std::time_t tt = std::chrono::system_clock::to_time_t(sctp);
    std::tm tm;

#if defined(_MSC_VER)
    localtime_s(&tm, &tt);
#else
    localtime_r(&tt, &tm);
#endif

    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
    return std::string(buf);
}

// Compute recursive totals for a directory (files, dirs, total size)
DirectoryTotals computeDirectoryTotals(const std::string& directory) {
    DirectoryTotals totals{0, 0, 0};
    try {
        fs::directory_options opts = fs::directory_options::skip_permission_denied;
        for (const auto& entry : fs::recursive_directory_iterator(directory, opts)) {
            std::error_code ec;
            if (entry.is_regular_file(ec) && !ec) {
                ++totals.totalFiles;
                std::uintmax_t s = 0;
                std::error_code e2;
                s = fs::file_size(entry.path(), e2);
                if (!e2) totals.totalSize += s;
            } else if (entry.is_directory(ec) && !ec) {
                ++totals.totalDirs;
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error while computing totals: " << e.what() << std::endl;
    }
    return totals;
}

std::vector<FileSummary> listImmediateFiles(const std::string& directory) {
    std::vector<FileSummary> out;
    try {
        fs::directory_options opts = fs::directory_options::skip_permission_denied;
        for (const auto& entry : fs::directory_iterator(directory, opts)) {
            std::error_code ec;
            if (!entry.is_regular_file(ec) || ec) continue;
            std::uintmax_t s = 0;
            std::error_code e2;
            s = fs::file_size(entry.path(), e2);
            if (e2) s = 0;
            FileSummary f;
            f.filename = entry.path().filename().string();
            f.extension = entry.path().extension().string();
            f.size = s;
            out.push_back(std::move(f));
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error while listing files: " << e.what() << std::endl;
    }
    // Sort by extension then filename for deterministic output
    std::sort(out.begin(), out.end(), [](const FileSummary& a, const FileSummary& b){
        if (a.extension != b.extension) return a.extension < b.extension;
        return a.filename < b.filename;
    });
    return out;
}

std::vector<SubdirSummary> listImmediateSubdirs(const std::string& directory) {
    std::vector<SubdirSummary> out;
    try {
        fs::directory_options opts = fs::directory_options::skip_permission_denied;
        for (const auto& entry : fs::directory_iterator(directory, opts)) {
            std::error_code ec;
            if (!entry.is_directory(ec) || ec) continue;
            SubdirSummary s;
            s.dirpath = entry.path().string();
            // Compute totals for this subdir recursively
            DirectoryTotals dt = computeDirectoryTotals(entry.path().string());
            s.totalSize = dt.totalSize;
            s.fileCount = dt.totalFiles;
            out.push_back(std::move(s));
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error while listing subdirectories: " << e.what() << std::endl;
    }
    // Sort by name
    std::sort(out.begin(), out.end(), [](const SubdirSummary& a, const SubdirSummary& b){
        return a.dirpath < b.dirpath;
    });
    return out;
}

// Run command for StatsTool
void StatsCommand::run() const {
    try {
        if (targetPath.empty()) {
            std::cerr << "Error: No path provided to stats command.\n";
            std::exit(1);
        }

        fs::path target(targetPath);
        if (!fs::exists(target)) {
            std::cerr << "Error: Path does not exist: " << targetPath << std::endl;
            std::exit(1);
        }

        if (fs::is_regular_file(target)) {
            // File mode
            std::uintmax_t size = 0;
            std::error_code ec;
            size = fs::file_size(target, ec);
            if (ec) size = 0;

            std::cout << "File: " << fs::absolute(target) << '\n';
            std::cout << "Type: " << target.extension().string() << '\n';
            std::cout << "Size: " << formatSize(size) << '\n';

            // Last modified
            std::error_code ec2;
            auto lw = fs::last_write_time(target, ec2);
            if (!ec2) {
                std::cout << "Last modified: " << formatFileTime(lw) << '\n';
            }
            return;
        }

        if (fs::is_directory(target)) {
            // Directory mode
            std::cout << "Directory: " << fs::absolute(target) << '\n';
            DirectoryTotals totals = computeDirectoryTotals(target.string());
            std::cout << "Total files (recursive): " << totals.totalFiles << '\n';
            std::cout << "Total directories (recursive): " << totals.totalDirs << '\n';
            std::cout << "Total size (recursive): " << formatSize(totals.totalSize) << '\n';
            std::cout << '\n';

            // Immediate files grouped by extension
            auto files = listImmediateFiles(target.string());
            if (!files.empty()) {
                std::cout << "Files (immediate):\n";
                std::string currentExt;
                for (const auto& f : files) {
                    if (f.extension != currentExt) {
                        currentExt = f.extension;
                        if (currentExt.empty()) std::cout << "  [no extension]\n";
                        else std::cout << "  " << currentExt << '\n';
                    }
                    std::cout << "    " << f.filename << " — " << formatSize(f.size) << '\n';
                }
                std::cout << '\n';
            } else {
                std::cout << "No immediate files in directory.\n\n";
            }

            // Immediate subdirectories summary
            auto subdirs = listImmediateSubdirs(target.string());
            if (!subdirs.empty()) {
                std::cout << "Subdirectories (immediate):\n";
                for (const auto& sd : subdirs) {
                    std::cout << "  " << sd.dirpath << '\n';
                    std::cout << "    Total size (recursive): " << formatSize(sd.totalSize) << '\n';
                    std::cout << "    File count (recursive): " << sd.fileCount << '\n';
                }
            } else {
                std::cout << "No immediate subdirectories.\n";
            }
            return;
        }

        // Fallback
        std::cerr << "Path is neither a regular file nor directory: " << targetPath << std::endl;
        std::exit(1);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error in stats run: " << e.what() << std::endl;
        std::exit(1);
    } catch (const std::exception& e) {
        std::cerr << "Error in stats run: " << e.what() << std::endl;
        std::exit(1);
    }
}