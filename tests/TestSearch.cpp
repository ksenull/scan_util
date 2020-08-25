#include <filesystem>
#include <fstream>
#include <iostream>

#include "../scan/Detect.h"
#include "../scan/DirectoryStats.h"

namespace fs = std::filesystem;

struct TestDataHolder {
    unsigned NJsFiles = 3;
    unsigned NJsDetects = 2;
    unsigned NUnixFiles = 3;
    unsigned NUnixDetects = 2;
    unsigned NMacosDetects = 1;
    unsigned MinFileSize_b = 2048;
};

std::string GenRandomString(unsigned len, bool newlines=false, const std::string& randInsert="") {
    std::string alphanum =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            " 0123456789";
    if (newlines) {
        alphanum += "\n";
    }

    const size_t randomInsertPos = rand() % len;
    std::string result;
    for (size_t i = 0; i < len; ++i) {
        if (i == randomInsertPos) {
            result += randInsert;
        }
        result += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return result;
}

std::string GenRandomExtension() {
    std::vector<std::string> extensions{".txt", "", ".sh"};
    const unsigned idx = rand() % extensions.size();
    return extensions[idx];
}

void GenRandomFile(const std::string& filename, unsigned filesize, const std::string& detect) {
    std::ofstream out(filename, std::ios::out);
    out << GenRandomString(filesize, true, detect);
    out.close();
}

void GenerateData(const fs::path& testDir, const TestDataHolder& testHolder) {
    if (!fs::exists(testDir)) {
        fs::create_directory(testDir);
    }

    const std::string jsDetectPattern = Detect{.Type = DetectType::Js}.GetPattern();
    for (unsigned i = 0; i < testHolder.NJsFiles; i++) {
        const std::string filename = testDir / (GenRandomString(8) + ".js");
        GenRandomFile(filename, testHolder.MinFileSize_b, (i < testHolder.NJsFiles) ? jsDetectPattern : "");
    }

    const std::string unixDetectPattern = Detect{.Type = DetectType::Unix}.GetPattern();
    const std::string macosDetectPattern = Detect{.Type = DetectType::Macos}.GetPattern();
    for (unsigned i = 0; i < testHolder.NUnixFiles; i++) {
        const std::string filename = testDir / (GenRandomString(8) + GenRandomExtension());
        auto detect = (i < testHolder.NUnixDetects) ? unixDetectPattern :
                      (i < (testHolder.NUnixDetects + testHolder.NMacosDetects)) ? macosDetectPattern : "";
        GenRandomFile(filename, testHolder.MinFileSize_b, detect);
    }
}

void CheckReport(const fs::path& testDir) {
    const auto scanStat = ScanDirectory(testDir);
    scanStat.Report();
}

void testSearch(const std::string& testDir) {
    const TestDataHolder testDataHolder;
    GenerateData(testDir, testDataHolder);
    CheckReport(testDir);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: ./TestSearch <test_dir>" << std::endl;
        return 1;
    }
    const fs::path testDir = argv[1];
    testSearch(testDir);
}

