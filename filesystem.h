#ifndef CPP_FILE_MANAGER_FILESYSTEM_H
#define CPP_FILE_MANAGER_FILESYSTEM_H

#include <string>
#include <vector>

struct FileEntry {
    std::string name;
    bool is_dir;
};

struct FileInfo {
    std::string name;
    bool is_dir;
    size_t size;
    mode_t mode;
};

std::vector<FileEntry> list_dir(const std::string& path);
FileInfo get_info(const std::string& path);
bool copy_file(const std::string& src, const std::string& dst);
bool move_file(const std::string& src, const std::string& dst);

#endif