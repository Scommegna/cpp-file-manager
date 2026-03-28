#ifndef CPP_FILE_MANAGER_FILESYSTEM_H
#define CPP_FILE_MANAGER_FILESYSTEM_H

#include <string>
#include <vector>

struct FileEntry {
    std::string name;
    bool is_dir;
};

std::vector<FileEntry> list_dir(const std::string& path);

#endif