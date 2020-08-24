#include "DirectoryScanner.h"

#include <iostream>

#include "FileScanner.h"

void DirectoryScanner::Scan() {
    const auto startTime = std::chrono::high_resolution_clock::now();
    for(auto& p: fs::directory_iterator(directoryPath)) {
        if (!p.is_regular_file()) {
            continue;
        }
        FileScanner fs(p.path());
        fs.Scan();
        nFilesScanned += 1;
        nJsDetects += fs.GetDetects(DetectType::Js);
        nUnixDetects += fs.GetDetects(DetectType::Unix);
        nMacosDetects += fs.GetDetects(DetectType::Macos);
    }
    const auto endTime = std::chrono::high_resolution_clock::now();
    executionTime = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
}

void DirectoryScanner::PrintReport() const {
    auto s = executionTime;
    auto h = std::chrono::duration_cast<std::chrono::hours>(s);
    s -= h;
    auto m = std::chrono::duration_cast<std::chrono::minutes>(s);
    s -= m;
    std::cout << "====== Scan result ======" << std::endl;
    std::cout << "Processed files: " << nFilesScanned << std::endl;
    std::cout << "JS detects: " << nJsDetects << std::endl;
    std::cout << "Unix detects: " << nUnixDetects << std::endl;
    std::cout << "macOS detects: " << nMacosDetects << std::endl;
    std::cout << "Errors: " << nErrors << std::endl;
    std::cout << "Execution time: " << std::setw(2) << std::setfill('0') << h.count() << ":" <<
                                       std::setw(2) << std::setfill('0') << m.count() << ":" <<
                                       std::setw(2) << std::setfill('0') << s.count() << std::endl;
    std::cout << "=========================" << std::endl;
}
