#include "tree.h"

#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <vector>
#include <algorithm>

void print_tree_recursive(const std::string &path, const std::string &prefix) {
    DIR* dir = opendir(path.c_str());

    if (!dir) {
        std::cout << "Could not open current directory." << std::endl;
        return;
    }

    std::vector<std::string> entries;

    struct dirent* entry;

    while ((entry = readdir(dir)) != nullptr) {
        std::string name = entry->d_name;

        if(name == "." || name == "..") continue;

        entries.push_back(name);
    }

    closedir(dir);

    std::sort(entries.begin(), entries.end());

    for(size_t i = 0; i < entries.size(); i++) {
        bool last = (i == entries.size() - 1);

        std::string name = entries[i];
        std::string full_path = path + "/" + name;

        std::cout << prefix;

        if (last) std::cout << "└── ";
        else std::cout << "├── ";

        std::cout << name << std::endl;

        struct stat sb;

        if (lstat(full_path.c_str(), &sb) != 0) continue;

        if (S_ISDIR(sb.st_mode)) {
            std::string next_prefix = prefix;

            if (last) next_prefix += "    ";
            else next_prefix += "│   ";

            print_tree_recursive(full_path, next_prefix);
        }
    }
}

void print_tree(const std::string &path) {
    std::cout << path << std::endl;
    print_tree_recursive(path, "");
}
