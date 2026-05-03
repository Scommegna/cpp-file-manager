#include "utils.h"
#include <iostream>
#include <sstream>
#include <sys/stat.h>

void print_files(const std::vector<FileEntry>& files) {
    for (const auto& file : files) {
        if (file.is_dir) {
            std::cout << "[DIR] ";
        } else {
            std::cout << "[FILE] ";
        }

        std::cout << file.name << std::endl;
    }
}

std::vector<std::string> split(const std::string& input) {
    std::stringstream ss(input);
    std::string word;
    std::vector<std::string> tokens;

    while (ss >> word) {
        tokens.push_back(word);
    }

    return tokens;
}

void print_info(const FileInfo& info) {
    std::cout << "Name: " << info.name << std::endl;
    std::cout << "Size: " << info.size << " bytes" << std::endl;
    std::cout << "Type: " << (info.is_dir ? "Directory" : "File") << std::endl;
    std::cout << "Permissions: " << format_permissions(info.mode) << std::endl;
}

std::string format_permissions(const mode_t mode) {
    std::string perms;

    perms += (mode & S_IRUSR) ? 'r' : '-';
    perms += (mode & S_IWUSR) ? 'w' : '-';
    perms += (mode & S_IXUSR) ? 'x' : '-';

    perms += (mode & S_IRGRP) ? 'r' : '-';
    perms += (mode & S_IWGRP) ? 'w' : '-';
    perms += (mode & S_IXGRP) ? 'x' : '-';

    perms += (mode & S_IROTH) ? 'r' : '-';
    perms += (mode & S_IWOTH) ? 'w' : '-';
    perms += (mode & S_IXOTH) ? 'x' : '-';

    return perms;
}

bool has_flag(const std::vector<std::string>& args, const std::string& flag) {
    for(const auto& arg : args) {
        if (arg == flag) {
            return true;
        }
    }

    return false;
}