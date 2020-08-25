#include "FileScanner.h"
#include "search/AhoCorasick.h"

#include <cassert>
#include <fstream>
#include <sstream>

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

    std::vector<Detect> allowedDetects;
    for (const auto& d: searchableDetects) {
        if (d.GetExtension().empty() || filepath.extension() == d.GetExtension()) {
            allowedDetects.emplace_back(d);
        }
    }

    std::ifstream infile(filepath);
    for (std::string line; std::getline(infile, line); ) {
        for (const auto& d: allowedDetects) {
            if (line.find(d.GetPattern()) != std::string::npos) {
                return d;
            }
        }
    }
    infile.close();
    return {};
}

Detect FileScanner::ahoCorasickSearch(const fs::path& filepath) {
    assert(searchMode == SearchMode::AhoCorasick);
    std::ifstream infile(filepath);
    std::stringstream buffer;
    buffer << infile.rdbuf();

    std::vector<bool> allowedDetects(searchableDetects.size(), false);
    for (size_t i = 0; i < searchableDetects.size(); i++) {
        if (searchableDetects[i].GetExtension().empty() || filepath.extension() == searchableDetects[i].GetExtension()) {
            allowedDetects[i] = true;
        }
    }
    int patternId = ahoCorasick->Find(buffer.str(), allowedDetects);

    infile.close();
    return (patternId >= 0) ? searchableDetects[patternId] : Detect{};
}
