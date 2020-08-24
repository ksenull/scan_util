#include "FileScanner.h"

#include <fstream>

void FileScanner::Scan() {
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
}
