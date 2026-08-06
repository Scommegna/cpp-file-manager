#include "benchmark.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <iomanip>

void Benchmark::start_benchmark() {
    start_time = std::chrono::steady_clock::now();
}

BenchmarkResult Benchmark::stop_benchmark() {
    auto end_time = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(end_time - start_time).count();

    BenchmarkResult result;

    result.elapsed_time_ms = elapsed;
    result.memory_usage_kb = get_memory_usage_kb();

    return result;
}

long get_memory_usage_kb() {
    std::ifstream file("/proc/self/status");

    if (!file.is_open()) {
        return -1;
    }

    std::string line;

    while (std::getline(file, line)) {
        if (line.rfind("VmRSS:", 0) == 0) {
            std::istringstream iss(line);

            std::string key;
            long value;
            std::string unit;

            iss >> key >> value >> unit;

            return value;
        }
    }

    return -1;
}

void print_benckmark(const BenchmarkResult &result) {
    std::cout << "\n[Benchmark]\n";
    std::cout << "Execution time: " << std::fixed << std::setprecision(3) << result.elapsed_time_ms << " ms\n";

    if (result.memory_usage_kb >= 0) {
        std::cout << "Current resident memory: " << result.memory_usage_kb << " kB\n";
    } else {
        std::cout << "Current resident memory: not available" << std::endl;
    }
}
