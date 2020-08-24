#include "FileScanner.h"
#include "AhoCorasick.h"

#include <cassert>
#include <fstream>

void FileScanner::Scan(FileScanner::SearchMode mode) {
    switch (mode) {
        case FileScanner::SearchMode::Simple:
            simpleSearch();
            break;
        case FileScanner::SearchMode::AhoCorasick:
            ahoCorasickSearch();
            break;
        default:
            assert(false);
    }
}

void FileScanner::simpleSearch() {
    std::ifstream infile(filepath);
    for (std::string line; std::getline(infile, line); ) {
        for (const auto& d: Detect::DetectTypeToRequirement) {
            if (filepath.extension() != d.second.Extension) {
                continue;
            }
            if (line.find(d.second.Pattern) != std::string::npos) {
                detect = Detect {.Type = d.first};
                return;
            }
        }
    }
    infile.close();
}

void FileScanner::ahoCorasickSearch() {
    AhoCorasickSearch search;
    for (const auto& d: Detect::DetectTypeToRequirement) {
        search.AddPattern(d.second.Pattern);
    }

    std::ifstream infile(filepath);
    for (std::string line; std::getline(infile, line);) {
        search.Find(line);
    }

    infile.close();
}
