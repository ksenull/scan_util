#pragma once

#include <chrono>
#include <filesystem>
#include <string>
#include <utility>


namespace fs = std::filesystem;

class DirectoryScanner {
public:
    explicit DirectoryScanner(std::string  path): directoryPath(std::move(path)) {}

    void Scan();

    void PrintReport() const;

private:
    const std::string directoryPath;
    int nFilesScanned = 0;
    int nJsDetects = 0;
    int nUnixDetects = 0;
    int nMacosDetects = 0;
    int nErrors = 0;
    std::chrono::duration<long> executionTime;
};