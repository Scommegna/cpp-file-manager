#ifndef CPP_FILE_MANAGER_SEARCH_H
#define CPP_FILE_MANAGER_SEARCH_H

#include <string>

void search_files(const std::string& term, const std::string& path);

void search_files_mt(const std::string& term, const std::string& root_path, unsigned int thread_count);

#endif