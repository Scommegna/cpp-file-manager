#include "filesystem.h"

#include <sys/stat.h>
#include <dirent.h>
#include <iostream>

std::vector<FileEntry> list_dir(const std::string &path) {
    std::vector<FileEntry> result;

    DIR* dir = opendir(path.c_str());

    if (!dir) {
        std::cout << "Failed to open dir " + path << std::endl;
        return result;
    }

    struct dirent* entry;

    while ((entry = readdir(dir)) != nullptr) {
        std::string name = entry->d_name;

        if (name == "." || name == "..") continue;

        FileEntry file;
        file.name = name;

        if (entry->d_type == DT_DIR) {
            file.is_dir = true;
        } else {
            file.is_dir = false;
        }

        result.push_back(file);
    }

    closedir(dir);
    return result;
}

FileInfo get_info(const std::string& path) {
    struct stat sb;

    if (stat(path.c_str(), &sb) != 0) {
        std::cout << "Failed to get info of " + path << std::endl;
        return {};
    }

    FileInfo info;
    info.name = path;
    info.size = sb.st_size;
    info.is_dir = S_ISDIR(sb.st_mode);
    info.mode = sb.st_mode;

    return info;
}
