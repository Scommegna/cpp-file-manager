#include "filesystem.h"

#include <sys/stat.h>
#include <dirent.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <limits>

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

bool copy_file(const std::string &src, const std::string &dst) {
    const size_t BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];

    int src_fd = open(src.c_str(), O_RDONLY);
    if (src_fd < 0) {
        std::cout << "Failed to open " << src << std::endl;
        return false;
    }

    int dst_fd = open(dst.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst_fd < 0) {
        std::cout << "Failed to open " << dst << std::endl;
        return false;
    }

    ssize_t bytes_read;

    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        ssize_t bytes_written = write(dst_fd, buffer, bytes_read);

        if (bytes_written != bytes_read) {
            std::cout << "Failed to write " << bytes_read << std::endl;
            close(src_fd);
            close(dst_fd);
            return false;
        }
    }

    if (bytes_read < 0) {
        std::cout << "Failed to read " << src << std::endl;
        return false;
    }

    close(src_fd);
    close(dst_fd);

    return true;
}

bool move_file(const std::string &src, const std::string &dst) {
    if (rename(src.c_str(), dst.c_str()) != 0) {
        std::cout << "Failed to move " << src << " to " << dst << std::endl;
        return false;
    }

    return true;
}

bool delete_path(const std::string &path) {
    struct stat sb;

    if (lstat(path.c_str(), &sb) != 0) {
      std::cout << "Failed to stat of " << path << std::endl;
        return false;
    }

    if (S_ISLNK(sb.st_mode)) {
        if (unlink(path.c_str()) != 0) {
            std::cout << "Failed to remove symlink " << path << std::endl;
            return false;
        }

        return true;
    }

    if (S_ISDIR(sb.st_mode)) {
        DIR* dir = opendir(path.c_str());

        if (!dir) {
            std::cout << "Failed to open " << path << std::endl;
            return false;
        }

        struct dirent* entry;

        while ((entry = readdir(dir)) != nullptr) {
            std::string name = entry->d_name;

            if (name == "." || name == "..") continue;

            std::string full_path = path + "/" + name;

            if (!delete_path(full_path)) {
                closedir(dir);
                return false;
            }
        }

        closedir(dir);

        if (rmdir(path.c_str()) != 0) {
            std::cout << "Failed to remove " << path << std::endl;
            return false;
        }
    } else {
        if (unlink(path.c_str()) != 0) {
            std::cout << "Failed to remove " << path << std::endl;
            return false;
        }
    }

    return true;
}

bool change_dir(const std::string &path) {
    if (chdir(path.c_str()) != 0) {
        std::cout << "Failed to chdir to " << path << std::endl;
        return false;
    }

    return true;
}

std::string get_current_dir() {
    char buffer[PATH_MAX];

    if (getcwd(buffer, sizeof(buffer)) == nullptr) {
        std::cout << "Error getting current directory " << buffer << std::endl;
        return "";
    }

    return std::string(buffer);
}
