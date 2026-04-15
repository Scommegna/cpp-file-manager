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

        if (args[0] == "exit") {
            break;
        }
        else if (args[0] == "list") {
            std::string path = (args.size() > 1) ? args[1] : ".";

            auto files = list_dir(path);
            print_files(files);
        }
        else if (args[0] == "info") {
            if (args.size() < 2) {
                std::cout << "Usage: info <file>" << std::endl;
                continue;
            }

            auto info = get_info(args[1]);
            print_info(info);
        }
        else if (args[0] == "copy") {
            if (args.size() < 3) {
                std::cout << "Usage: copy <src> <dst>" << std::endl;
                continue;
            }

            if (copy_file(args[1], args[2])) {
                std::cout << "Copied " << args[1] << " to " << args[2] << std::endl;
            } else {
                std::cout << "Error to copy " << args[1] << " from " << args[2] << std::endl;
            }
        }
        else {
            std::cout << "Unknown command: " << args[0] << std::endl;
        }
    }

    return 0;
}