#include "search.h"
#include "filesystem.h"

#include <dirent.h>
#include <sys/stat.h>
#include <algorithm>
#include <condition_variable>
#include <cerrno>
#include <cstring>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

namespace {

DIR* open_root_directory(const std::string& path) {
    struct stat sb;
    if (lstat(path.c_str(), &sb) != 0) {
        int error = errno;
        throw std::runtime_error("cannot access initial directory '" + path + "': " + std::strerror(error));
    }
    if (!S_ISDIR(sb.st_mode)) {
        throw std::runtime_error("initial path is not a directory: " + path);
    }

    DIR* dir = opendir(path.c_str());
    if (!dir) {
        int error = errno;
        throw std::runtime_error("cannot open initial directory '" + path + "': " + std::strerror(error));
    }
    return dir;
}

void search_directory(const std::string& term, const std::string& path, DIR* dir,
                      std::vector<std::string>& results) {

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string name = entry->d_name;
        if (name == "." || name == "..") continue;

        std::string full_path = join_path(path, name);
        if (name.find(term) != std::string::npos) results.push_back(full_path);

        struct stat sb;
        if (lstat(full_path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
            DIR* subdir = opendir(full_path.c_str());
            if (subdir) search_directory(term, full_path, subdir, results);
        }
    }
    closedir(dir);
}

}

std::vector<std::string> search_files(const std::string& term, const std::string& path) {
    std::vector<std::string> results;
    DIR* dir = open_root_directory(path);
    search_directory(term, path, dir, results);
    std::sort(results.begin(), results.end());
    return results;
}

std::vector<std::string> search_files_mt(const std::string& term, const std::string& root_path, unsigned int thread_count) {
    if (thread_count <= 1) return search_files(term, root_path);

    DIR* root = open_root_directory(root_path);
    closedir(root);

    std::queue<std::string> directories;
    std::vector<std::string> results;
    std::mutex work_mutex;
    std::mutex results_mutex;
    std::condition_variable cv;
    std::size_t pending = 1;
    bool done = false;
    directories.push(root_path);

    auto worker = [&]() {
        while (true) {
            std::string current_path;
            {
                std::unique_lock<std::mutex> lock(work_mutex);
                cv.wait(lock, [&]() { return done || !directories.empty(); });
                if (done && directories.empty()) return;
                current_path = directories.front();
                directories.pop();
            }

            DIR* dir = opendir(current_path.c_str());
            if (dir) {
                struct dirent* entry;
                while ((entry = readdir(dir)) != nullptr) {
                    std::string name = entry->d_name;
                    if (name == "." || name == "..") continue;

                    std::string full_path = join_path(current_path, name);
                    if (name.find(term) != std::string::npos) {
                        std::lock_guard<std::mutex> lock(results_mutex);
                        results.push_back(full_path);
                    }

                    struct stat sb;
                    if (lstat(full_path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
                        {
                            std::lock_guard<std::mutex> lock(work_mutex);
                            directories.push(full_path);
                            ++pending;
                        }
                        cv.notify_one();
                    }
                }
                closedir(dir);
            }

            {
                std::lock_guard<std::mutex> lock(work_mutex);
                --pending;
                if (pending == 0) {
                    done = true;
                    cv.notify_all();
                }
            }
        }
    };

    std::vector<std::thread> threads;
    try {
        for (unsigned int i = 0; i < thread_count; ++i) threads.emplace_back(worker);
    } catch (...) {
        {
            std::lock_guard<std::mutex> lock(work_mutex);
            done = true;
            while (!directories.empty()) directories.pop();
        }
        cv.notify_all();
        for (auto& thread : threads) thread.join();
        throw;
    }

    cv.notify_all();
    for (auto& thread : threads) thread.join();
    std::sort(results.begin(), results.end());
    return results;
}
