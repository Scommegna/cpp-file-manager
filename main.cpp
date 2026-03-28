#include <iostream>
#include <vector>
#include "filesystem.h"
#include "utils.h"



int main() {
    std::string input;

    while (true) {
        std::cout << "fm> ";
        std::getline(std::cin, input);

        auto args = split(input);

        if (args.empty()) continue;

        if (args[0] == "exit") break;

        if (args[0] == "list") {
            std::string path = (args.size() > 1) ? args[1] : ".";

            auto files = list_dir(path);
            print_files(files);
        } else {
            std::cout << "Unknown command: " << args[0] << std::endl;
        }
    }

    return 0;
}