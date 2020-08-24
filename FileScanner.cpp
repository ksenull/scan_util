#include "FileScanner.h"
#include "AhoCorasick.h"

#include <cassert>
#include <fstream>

Detect FileScanner::Scan(const fs::path& filepath) {
    switch (searchMode) {
        case FileScanner::SearchMode::Simple:
            return simpleSearch(filepath);
        case FileScanner::SearchMode::AhoCorasick:
            return ahoCorasickSearch(filepath);
        default:
            assert(false);
    }
}

Detect FileScanner::simpleSearch(const fs::path& filepath) {
    assert(searchMode == SearchMode::Simple);

    std::ifstream infile(filepath);
    for (std::string line; std::getline(infile, line); ) {
        for (const auto& d: DetectTypeToDetect) {
            if (filepath.extension() != d.second.Extension) {
                continue;
            }
            if (line.find(d.second.Pattern) != std::string::npos) {
                auto detect = Detect {.Type = d.first};
                return detect;
            }
        }
    }
    infile.close();
    return {};
}

Detect FileScanner::ahoCorasickSearch(const fs::path& filepath) {
    assert(searchMode == SearchMode::AhoCorasick);
    std::ifstream infile(filepath);
    std::string str((std::istreambuf_iterator<char>(infile)),
                    std::istreambuf_iterator<char>());
    int patternId = ahoCorasick->Find(str);

    infile.close();
    return (patternId >= 0) ? detectRequirements[patternId] : Detect{};
}
