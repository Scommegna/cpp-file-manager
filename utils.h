#ifndef CPP_FILE_MANAGER_UTILS_H
#define CPP_FILE_MANAGER_UTILS_H

#include "filesystem.h"
#include <string>
#include <vector>

void print_files(const std::vector<FileEntry>& files);
std::vector<std::string> split(const std::string& input);

#endif