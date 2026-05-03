#include <iostream>
#include <thread>
#include <vector>
#include "filesystem.h"
#include "tree.h"
#include "utils.h"
#include "search.h"
#include "benchmark.h"

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
            bool benchmark_enabled = has_flag(args, "--benchmark") || has_flag(args, "-b");

            Benchmark benchmark;

            if (benchmark_enabled) benchmark.start_benchmark();

            std::string path = (args.size() > 1) ? args[1] : ".";

            auto files = list_dir(path);
            print_files(files);

            if (benchmark_enabled) {
                auto result = benchmark.stop_benchmark();
                print_benckmark(result);
            }
        }
        else if (args[0] == "info") {
            if (args.size() < 2) {
                std::cout << "Usage: info <file>" << std::endl;
                continue;
            }

            bool benchmark_enabled = has_flag(args, "--benchmark") || has_flag(args, "-b");

            Benchmark benchmark;

            if (benchmark_enabled) benchmark.start_benchmark();

            auto info = get_info(args[1]);
            print_info(info);

            if (benchmark_enabled) {
                auto result = benchmark.stop_benchmark();
                print_benckmark(result);
            }
        }
        else if (args[0] == "copy") {
            if (args.size() < 3) {
                std::cout << "Usage: copy <src> <dst>" << std::endl;
                continue;
            }

            bool benchmark_enabled = has_flag(args, "--benchmark") || has_flag(args, "-b");

            Benchmark benchmark;

            if (benchmark_enabled) benchmark.start_benchmark();

            if (copy_file(args[1], args[2])) {
                std::cout << "Copied " << args[1] << " to " << args[2] << std::endl;
            } else {
                std::cout << "Error to copy " << args[1] << " from " << args[2] << std::endl;
            }

            if (benchmark_enabled) {
                auto result = benchmark.stop_benchmark();
                print_benckmark(result);
            }
        }
        else if (args[0] == "move") {
            if (args.size() < 3) {
                std::cout << "Usage: move <src> <dst>" << std::endl;
                continue;
            }

            bool benchmark_enabled = has_flag(args, "--benchmark") || has_flag(args, "-b");

            Benchmark benchmark;

            if (benchmark_enabled) benchmark.start_benchmark();

            if (move_file(args[1], args[2])) {
                std::cout << "Moved " << args[1] << " to " << args[2] << std::endl;
            } else {
                std::cout << "Error to move " << args[1] << " from " << args[2] << std::endl;
            }

            if (benchmark_enabled) {
                auto result = benchmark.stop_benchmark();
                print_benckmark(result);
            }
        }
        else if (args[0] == "delete") {
            if (args.size() < 2) {
                std::cout << "Usage: delete <src>" << std::endl;
                continue;
            }

            bool benchmark_enabled = has_flag(args, "--benchmark") || has_flag(args, "-b");

            Benchmark benchmark;

            if (benchmark_enabled) benchmark.start_benchmark();

            if (delete_path(args[1])) {
                std::cout << "Deleted " << args[1] << " successfully." << std::endl;
            } else {
                std::cout << "Error to delete " << args[1] << std::endl;
            }

            if (benchmark_enabled) {
                auto result = benchmark.stop_benchmark();
                print_benckmark(result);
            }
        }
        else if (args[0] == "cd") {
            if (args.size() < 2) {
                std::cout << "Usage: cd <src>" << std::endl;
                continue;
            }

            bool benchmark_enabled = has_flag(args, "--benchmark") || has_flag(args, "-b");

            Benchmark benchmark;

            if (benchmark_enabled) benchmark.start_benchmark();

            if (!change_dir(args[1])) {
                std::cout << "Error to change directory " << args[1] << std::endl;
            }

            if (benchmark_enabled) {
                auto result = benchmark.stop_benchmark();
                print_benckmark(result);
            }
        }
        else if (args[0] == "pwd") {
            bool benchmark_enabled = has_flag(args, "--benchmark") || has_flag(args, "-b");

            Benchmark benchmark;

            if (benchmark_enabled) benchmark.start_benchmark();

            std::cout << get_current_dir() << std::endl;

            if (benchmark_enabled) {
                auto result = benchmark.stop_benchmark();
                print_benckmark(result);
            }
        }
        else if (args[0] == "tree") {
            bool benchmark_enabled = has_flag(args, "--benchmark") || has_flag(args, "-b");

            Benchmark benchmark;

            if (benchmark_enabled) benchmark.start_benchmark();

            std::string path = (args.size() > 1) ? args[1] : ".";

            print_tree(path);

            if (benchmark_enabled) {
                auto result = benchmark.stop_benchmark();
                print_benckmark(result);
            }
        }
        else if (args[0] == "search") {
            if (args.size() < 2) {
                std::cout << "Usage: search <term> [path]" << std::endl;
                continue;
            }

            bool benchmark_enabled = has_flag(args, "--benchmark") || has_flag(args, "-b");

            Benchmark benchmark;

            if (benchmark_enabled) benchmark.start_benchmark();

            std::string term = args[1];
            std::string path = (args.size() > 2) ? args[2] : ".";

            search_files(term, path);

            if (benchmark_enabled) {
                auto result = benchmark.stop_benchmark();
                print_benckmark(result);
            }
        }
        else if (args[0] == "search-mt") {
            if (args.size() < 2) {
                std::cout << "Usage: search-mt <term> [path] [threads]" << std::endl;
                continue;
            }

            bool benchmark_enabled = has_flag(args, "--benchmark") || has_flag(args, "-b");

            Benchmark benchmark;

            if (benchmark_enabled) benchmark.start_benchmark();

            std::string term = args[1];
            std::string path = (args.size() > 2) ? args[2] : ".";

            unsigned int threads = std::thread::hardware_concurrency();

            if (args.size() > 3) {
                threads = std::stoi(args[3]);
            }

            search_files_mt(term, path, threads);

            if (benchmark_enabled) {
                auto result = benchmark.stop_benchmark();
                print_benckmark(result);
            }
        }
        else {
            std::cout << "Unknown command: " << args[0] << std::endl;
        }
    }

    return 0;
}