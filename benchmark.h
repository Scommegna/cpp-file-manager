#ifndef CPP_FILE_MANAGER_BENCHMARK_H
#define CPP_FILE_MANAGER_BENCHMARK_H

#include <chrono>
#include <string>

struct BenchmarkResult {
    long elapsed_time_ms;
    long memory_usage_kb;
};

class Benchmark {
    private:
        std::chrono::high_resolution_clock::time_point start_time;

    public:
        void start_benchmark();
        BenchmarkResult stop_benchmark();
};

long get_memory_usage_kb();
void print_benckmark(const BenchmarkResult& result);

#endif