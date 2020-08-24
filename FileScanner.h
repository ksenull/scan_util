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
                for (size_t i = 0; i < searchableDetects.size(); i++) {
                    ahoCorasick->AddPattern(searchableDetects[i].GetPattern(), i);
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
    std::vector<Detect> searchableDetects {
            Detect{.Type = DetectType::Js},
            Detect{.Type = DetectType::Unix},
            Detect{.Type = DetectType::Macos}
    };
};

