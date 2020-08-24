#pragma once

#include <chrono>
#include <filesystem>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "Detect.h"


namespace fs = std::filesystem;

struct DirectoryStats {
    int NFilesScanned = 0;
    int NJsDetects = 0;
    int NUnixDetects = 0;
    int NMacosDetects = 0;
    int NErrors = 0;

    std::chrono::duration<long, std::ratio<1, 1000>> executionTime;

    void AddDetect(const Detect& detect);

    void Report() const;
};

class DirectoryScanner {
public:
    DirectoryStats Scan(const std::string& path);

private:
    unsigned int nThreads = std::thread::hardware_concurrency();
};