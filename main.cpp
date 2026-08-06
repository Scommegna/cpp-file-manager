#include <iostream>
#include <thread>
#include <vector>
#include <charconv>
#include <limits>
#include <exception>
#include "filesystem.h"
#include "tree.h"
#include "utils.h"
#include "search.h"
#include "benchmark.h"

bool parse_thread_count(const std::string& value, unsigned int& thread_count) {
    if (value.empty() || value.front() == '-') return false;
    unsigned long long parsed = 0;
    const char* begin = value.data();
    const char* end = begin + value.size();
    auto conversion = std::from_chars(begin, end, parsed);
    if (conversion.ec != std::errc() || conversion.ptr != end ||
        parsed > std::numeric_limits<unsigned int>::max()) {
        return false;
    }
    thread_count = static_cast<unsigned int>(parsed);
    return true;
}

void print_search_results(const std::vector<std::string>& results) {
    for (const auto& path : results) std::cout << path << std::endl;
}

int main() {
    std::string input;

    while (true) {
        std::cout << "fm> ";
        if (!std::getline(std::cin, input)) break;

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
            bool copied = copy_file(args[1], args[2]);
            BenchmarkResult result{};
            if (benchmark_enabled) result = benchmark.stop_benchmark();

            if (copied) {
                std::cout << "Copied " << args[1] << " to " << args[2] << std::endl;
            } else {
                std::cout << "Error to copy " << args[1] << " from " << args[2] << std::endl;
            }

            if (benchmark_enabled) print_benckmark(result);
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
            bool benchmark_enabled = has_flag(args, "--benchmark") || has_flag(args, "-b");
            std::vector<std::string> positional;
            for (std::size_t i = 1; i < args.size(); ++i) {
                if (args[i] != "--benchmark" && args[i] != "-b") positional.push_back(args[i]);
            }
            if (positional.size() < 2 || positional.size() > 3) {
                std::cout << "Usage: search <term> <directory> [threads]" << std::endl;
                continue;
            }

            unsigned int threads = 1;
            if (positional.size() == 3 && !parse_thread_count(positional[2], threads)) {
                std::cout << "Invalid thread count: " << positional[2] << std::endl;
                continue;
            }
            if (threads == 0) {
                std::cout << "Thread count must be greater than zero." << std::endl;
                continue;
            }

            Benchmark benchmark;
            if (benchmark_enabled) benchmark.start_benchmark();
            std::vector<std::string> results;
            try {
                results = threads == 1
                    ? search_files(positional[0], positional[1])
                    : search_files_mt(positional[0], positional[1], threads);
            } catch (const std::exception& error) {
                std::cout << "Failed to execute search: " << error.what() << std::endl;
                continue;
            }
            BenchmarkResult result{};
            if (benchmark_enabled) result = benchmark.stop_benchmark();
            if (benchmark_enabled) {
                std::cout << "Matches: " << results.size() << std::endl;
                print_benckmark(result);
            } else {
                print_search_results(results);
            }
        }
        else if (args[0] == "search-mt") {
            bool benchmark_enabled = has_flag(args, "--benchmark") || has_flag(args, "-b");
            std::vector<std::string> positional;
            for (std::size_t i = 1; i < args.size(); ++i) {
                if (args[i] != "--benchmark" && args[i] != "-b") positional.push_back(args[i]);
            }
            if (positional.size() < 2 || positional.size() > 3) {
                std::cout << "Usage: search-mt <term> <directory> [threads]" << std::endl;
                continue;
            }

            unsigned int threads = std::thread::hardware_concurrency();
            if (threads == 0) threads = 1;
            if (positional.size() == 3 && !parse_thread_count(positional[2], threads)) {
                std::cout << "Invalid thread count: " << positional[2] << std::endl;
                continue;
            }
            if (threads == 0) {
                std::cout << "Thread count must be greater than zero." << std::endl;
                continue;
            }

            Benchmark benchmark;
            if (benchmark_enabled) benchmark.start_benchmark();
            std::vector<std::string> results;
            try {
                results = threads == 1
                    ? search_files(positional[0], positional[1])
                    : search_files_mt(positional[0], positional[1], threads);
            } catch (const std::exception& error) {
                std::cout << "Failed to execute search: " << error.what() << std::endl;
                continue;
            }
            BenchmarkResult result{};
            if (benchmark_enabled) result = benchmark.stop_benchmark();
            if (benchmark_enabled) {
                std::cout << "Matches: " << results.size() << std::endl;
                print_benckmark(result);
            } else {
                print_search_results(results);
            }
        }
        else {
            std::cout << "Unknown command: " << args[0] << std::endl;
        }
    }

    return 0;
}
