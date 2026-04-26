#include "search.h"

#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

void search_recursive(const std::string& term, const std::string& path) {
    DIR* dir = opendir(path.c_str());

    if (!dir) return;

    struct dirent* entry;

    while ((entry = readdir(dir)) != nullptr) {
        std::string name = entry->d_name;

        if (name == "." || name == "..") continue;

        std::string full_path = path + "/" + name;

        if (name.find(term) != std::string::npos) {
            std::cout << full_path << std::endl;
        }

        struct stat sb;

        if (lstat(full_path.c_str(), &sb) != 0) continue;

        if (S_ISDIR(sb.st_mode)) {
            search_recursive(term, full_path);
        }
    }

    closedir(dir);
}

void search_files(const std::string& term, const std::string& path) {
    search_recursive(term, path);
}

void search_files_mt(const std::string &term, const std::string &root_path, unsigned int thread_count) {
    if (thread_count <= 1) {
        search_files(term, root_path);
        return;
    }

    std::queue<std::string> directories;
    std::mutex queue_mutex;
    std::mutex cout_mutex;
    std::condition_variable cv;

    std::atomic<int> active_tasks{0};
    bool done = false;

    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        directories.push(root_path);
        active_tasks++;
    }

    auto worker = [&]() {
        while (true) {
            std::string current_path;

            {
                std::unique_lock<std::mutex> lock(queue_mutex);

                cv.wait(lock, [&]() {return !directories.empty() || done; });

                if (done && directories.empty()) return;

                current_path = directories.front();
                directories.pop();
            }

            DIR* dir = opendir(current_path.c_str());

            if (dir) {
                struct dirent* entry;

                while ((entry = readdir(dir)) != nullptr) {
                    std::string name = entry->d_name;

                    if (name == "." || name == "..") {
                        continue;
                    }

                    std::string full_path = current_path;
                    if (!full_path.empty() && full_path.back() != '/') {
                        full_path += "/";
                    }
                    full_path += name;

                    if (name.find(term) != std::string::npos) {
                        std::lock_guard<std::mutex> cout_lock(cout_mutex);
                        std::cout << full_path << std::endl;
                    }

                    struct stat sb;

                    if (lstat(full_path.c_str(), &sb) != 0) continue;

                    if (S_ISDIR(sb.st_mode)) {
                        {
                            std::lock_guard<std::mutex> lock(queue_mutex);
                            directories.push(full_path);
                            active_tasks++;
                        }
                        cv.notify_one();
                    }
                }
                closedir(dir);
            }
            active_tasks--;

            if (active_tasks == 0) {
                std::lock_guard<std::mutex> lock(queue_mutex);
                if (directories.empty()) {
                    done = true;
                    cv.notify_all();
                }
            }
        }
    };

    std::vector<std::thread> threads;

    for (unsigned int i = 0; i < thread_count; i++) {
        threads.emplace_back(worker);
    }

    cv.notify_all();

    for (auto& t : threads) {
        t.join();
    }
}
