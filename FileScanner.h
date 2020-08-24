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
    explicit FileScanner(const fs::path& _filepath): filepath(_filepath) {}

    void Scan();

    int GetDetects(DetectType type) const {
        if (detect.has_value() && detect.value().Type == type) {
            return 1;
        }
        return 0;
    }
private:
    fs::path filepath;
    std::optional<Detect> detect;
};

