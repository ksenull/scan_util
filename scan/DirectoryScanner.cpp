#include "DirectoryScanner.h"

#include <iostream>
#include <future>

#include "FileScanner.h"
#include "Detect.h"


//int process_files(std::vector<std::string>* const files_p, std::mutex* const mutex_p)
//{
//    auto count = 0;
//    while (true)
//    {
//        std::string next {};
//        {
//            const std::unique_lock<std::mutex> lck {*mutex_p};
//            if (files_p->empty())  // nothing left to do
//                return count;
//            next = std::move(files_p->back());
//            files_p->pop_back();
//        }
//        FileScanner fs(p.path());
//        fs.Scan();
//    }
//}

DirectoryStats DirectoryScanner::Scan(const std::string& directoryPath) {
    DirectoryStats stats;

    const auto startTime = std::chrono::high_resolution_clock::now();

    const auto& directory_iterator = fs::directory_iterator(directoryPath);
    std::vector<std::future<Detect>> futures;
    std::for_each(fs::begin(directory_iterator), fs::end(directory_iterator), [&](const auto& entry){
        if (!entry.is_regular_file()) {
            return;
        }
        futures.push_back(std::async(
                std::launch::async, [](const auto& path) {
                    FileScanner fs(FileScanner::SearchMode::AhoCorasick);
                    return fs.Scan(path);
                }, entry.path()));
    });
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
