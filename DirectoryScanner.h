#pragma once

#include <chrono>
#include <filesystem>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>


namespace fs = std::filesystem;

class DirectoryScanner {
public:
    explicit DirectoryScanner(std::string  path): directoryPath(std::move(path)) {}

    void Scan();

    void PrintReport() const;

private:
    int nFilesScanned = 0;
    int nJsDetects = 0;
    int nUnixDetects = 0;
    int nMacosDetects = 0;
    int nErrors = 0;
    unsigned int nThreads = std::thread::hardware_concurrency();

    std::chrono::duration<long, std::ratio<1, 1000>> executionTime;

    const std::string directoryPath;

    std::mutex queueMutex;
    std::vector<std::thread> threads;
    std::vector<std::string> jobs;

};