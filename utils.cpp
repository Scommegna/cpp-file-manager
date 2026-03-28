#include "utils.h"
#include <iostream>
#include <sstream>

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