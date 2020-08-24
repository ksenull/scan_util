#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Detect.h"

namespace fs = std::filesystem;

class FileScanner {
public:
    enum class SearchMode {
        Simple,
        AhoCorasick
    };
    explicit FileScanner(const fs::path& _filepath): filepath(_filepath) {}

    void Scan(SearchMode mode = SearchMode::Simple);

    int GetDetects(DetectType type) const {
        if (detect.has_value() && detect.value().Type == type) {
            return 1;
        }
        return 0;
    }
private:
    void simpleSearch();
    void ahoCorasickSearch();

    fs::path filepath;
    std::optional<Detect> detect;
};

