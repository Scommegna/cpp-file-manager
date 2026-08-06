#ifndef CPP_FILE_MANAGER_SEARCH_H
#define CPP_FILE_MANAGER_SEARCH_H

#include <string>
#include <vector>

std::vector<std::string> search_files(const std::string& term, const std::string& path);

std::vector<std::string> search_files_mt(const std::string& term, const std::string& root_path, unsigned int thread_count);

#endif
