#ifndef CPP_FILE_MANAGER_UTILS_H
#define CPP_FILE_MANAGER_UTILS_H

#include "filesystem.h"
#include <string>
#include <vector>

void print_files(const std::vector<FileEntry>& files);
std::vector<std::string> split(const std::string& input);
void print_info(const FileInfo& info);
std::string format_permissions(mode_t mode);
bool has_flag(const std::vector<std::string>& args, const std::string& flag);

#endif