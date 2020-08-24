#pragma once

#include <cassert>
#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Detect.h"
#include "AhoCorasick.h"

namespace fs = std::filesystem;

class FileScanner {
public:
    enum class SearchMode {
        Simple,
        AhoCorasick
    };
    explicit FileScanner(SearchMode mode = SearchMode::Simple) : searchMode(mode) {
        switch (searchMode) {
            case SearchMode::Simple:
                break;
            case SearchMode::AhoCorasick:
                ahoCorasick = AhoCorasickSearch();
                for (const auto& d: DetectTypeToDetect) {
                    ahoCorasick->AddPattern(d.second.Pattern, detectRequirements.size());
                    detectRequirements.emplace_back(d.second);
                }
                break;
            default:
                assert(false);
        }
    }
    Detect Scan(const fs::path& filepath);

private:
    Detect simpleSearch(const fs::path& filepath);
    Detect ahoCorasickSearch(const fs::path& filepath);

    SearchMode searchMode;
    std::optional<AhoCorasickSearch> ahoCorasick;
    std::vector<Detect> detectRequirements;
};

