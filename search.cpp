#include "search.h"

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>
#include <string>

void search_recursive(const std::string& term, const std::string& path) {
    DIR* dir = opendir(path.c_str());

    if (!dir) return;

    struct dirent* entry;

    while ((entry = readdir(dir)) != nullptr) {
        std::string name = entry->d_name;

        if (name == "." || name == "..") continue;

        std::string full_path = path + "/" + name;

        if (name.find(term) != std::string::npos) {
            std::cout << full_path << std::endl;
        }

        struct stat sb;

        if (lstat(full_path.c_str(), &sb) != 0) continue;

        if (S_ISDIR(sb.st_mode)) {
            search_recursive(term, full_path);
        }
    }

    closedir(dir);
}

void search_files(const std::string& term, const std::string& path) {
    search_recursive(term, path);
}