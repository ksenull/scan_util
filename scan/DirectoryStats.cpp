#include "DirectoryStats.h"

#include <iostream>
#include <future>

#include "Detect.h"
#include "FileScanner.h"


DirectoryStats ScanDirectory(const std::string& directoryPath) {
    DirectoryStats stats;

    const auto startTime = std::chrono::high_resolution_clock::now();

    std::vector<std::future<Detect>> futures;
    // TODO check directory access
    for (const auto& entry: fs::directory_iterator(directoryPath)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        futures.push_back(std::async(
                std::launch::async, [](const auto& path) {
                    FileScanner fs(FileScanner::SearchMode::AhoCorasick);
                    return fs.Scan(path);
                }, entry.path()));
    }
    for (auto& future: futures) {
        try {
            stats.AddDetect(future.get());
        } catch (...) {
            stats.NErrors += 1;
        }
    }

    const auto endTime = std::chrono::high_resolution_clock::now();
    stats.executionTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    return stats;
}

void DirectoryStats::AddDetect(const Detect& detect) {
    NFilesScanned += 1;
    switch (detect.Type) {
        case DetectType::Js:
            NJsDetects += 1;
            break;
        case DetectType::Unix:
            NUnixDetects += 1;
            break;
        case DetectType::Macos:
            NMacosDetects += 1;
            break;
        case DetectType::None:
            break;
        default:
            assert(false);
    }
}

void DirectoryStats::Report() const {
    auto ms = executionTime;
    auto h = std::chrono::duration_cast<std::chrono::hours>(ms);
    ms -= h;
    auto m = std::chrono::duration_cast<std::chrono::minutes>(ms);
    ms -= m;
    auto s = std::chrono::duration_cast<std::chrono::seconds>(ms);
    ms -= s;
    std::cout << "====== Scan result ======" << std::endl;
    std::cout << "Processed files: " << NFilesScanned << std::endl;
    std::cout << "JS detects: " << NJsDetects << std::endl;
    std::cout << "Unix detects: " << NUnixDetects << std::endl;
    std::cout << "macOS detects: " << NMacosDetects << std::endl;
    std::cout << "Errors: " << NErrors << std::endl;
    std::cout << "Execution time: " << std::setw(2) << std::setfill('0') << h.count() << ":" <<
                                       std::setw(2) << std::setfill('0') << m.count() << ":" <<
                                       std::setw(2) << std::setfill('0') << s.count() << " " <<
                                       ms.count() << std::endl;
    std::cout << "=========================" << std::endl;
}
