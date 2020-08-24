#include "DirectoryScanner.h"

#include <iostream>

#include "FileScanner.h"


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

void DirectoryScanner::Scan() {
    const auto startTime = std::chrono::high_resolution_clock::now();

    for (auto &p: fs::directory_iterator(directoryPath)) {
        if (!p.is_regular_file()) {
            continue;
        }
//        jobs.emplace_back(p.path());

        FileScanner fs(p.path());
        fs.Scan(FileScanner::SearchMode::AhoCorasick);
        nFilesScanned += 1;
        nJsDetects += fs.GetDetects(DetectType::Js);
        nUnixDetects += fs.GetDetects(DetectType::Unix);
        nMacosDetects += fs.GetDetects(DetectType::Macos);
    }

//    threads.reserve(nThreads);
//    for (auto t = 0U; t < nThreads; t++) {
//        threads.emplace_back(process_files, &jobs, &queueMutex);
//    }

//    for (auto&& t: threads) {
//        t.join();
//    }

    const auto endTime = std::chrono::high_resolution_clock::now();
    executionTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
}

void DirectoryScanner::PrintReport() const {
    auto ms = executionTime;
    auto h = std::chrono::duration_cast<std::chrono::hours>(ms);
    ms -= h;
    auto m = std::chrono::duration_cast<std::chrono::minutes>(ms);
    ms -= m;
    auto s = std::chrono::duration_cast<std::chrono::seconds>(ms);
    ms -= s;
    std::cout << "====== Scan result ======" << std::endl;
    std::cout << "Processed files: " << nFilesScanned << std::endl;
    std::cout << "JS detects: " << nJsDetects << std::endl;
    std::cout << "Unix detects: " << nUnixDetects << std::endl;
    std::cout << "macOS detects: " << nMacosDetects << std::endl;
    std::cout << "Errors: " << nErrors << std::endl;
    std::cout << "Execution time: " << std::setw(2) << std::setfill('0') << h.count() << ":" <<
                                       std::setw(2) << std::setfill('0') << m.count() << ":" <<
                                       std::setw(2) << std::setfill('0') << s.count() << " " <<
                                       ms.count() << std::endl;
    std::cout << "=========================" << std::endl;
}
