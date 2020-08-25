#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "../scan/Detect.h"
#include "../scan/DirectoryStats.h"

namespace fs = std::filesystem;

struct TestDataHolder {
    unsigned NJsFiles = 500;
    unsigned NJsDetects = 200;
    unsigned NUnixFiles = 500;
    unsigned NUnixDetects = 300;
    unsigned NMacosDetects = 100;
    unsigned MinFileSize_b = 2048;
};

std::string GenRandomString(unsigned len, bool newlines=false, const std::string& randInsert="") {
    std::string alphanum = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz 0123456789";
    if (newlines) {
        alphanum += "\n";
    }

    const size_t randomInsertPos = rand() % len;
    std::string result;
    for (size_t i = 0; i < len; ++i) {
        if (i == randomInsertPos) {
            result += randInsert;
        }
        result += alphanum[rand() % (alphanum.length() - 1)];
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

class SearchTest {
public:
    void Test(const fs::path& testDir) {
        cleanPreviousData(testDir);
        generateData(testDir);
        checkReport(testDir);
    }

private:
    void cleanPreviousData(const fs::path& testDir) {
        fs::remove_all(testDir);
    }
    
    void generateData(const fs::path& testDir) {
        if (!fs::exists(testDir)) {
            fs::create_directory(testDir);
        }

        const std::string jsDetectPattern = Detect{.Type = DetectType::Js}.GetPattern();
        for (unsigned i = 0; i < testDataHolder.NJsFiles; i++) {
            const std::string filename = testDir / (GenRandomString(8) + ".js");
            GenRandomFile(filename, testDataHolder.MinFileSize_b, (i < testDataHolder.NJsDetects) ? jsDetectPattern : "");
        }

        const std::string unixDetectPattern = Detect{.Type = DetectType::Unix}.GetPattern();
        const std::string macosDetectPattern = Detect{.Type = DetectType::Macos}.GetPattern();
        for (unsigned i = 0; i < testDataHolder.NUnixFiles; i++) {
            const std::string filename = testDir / (GenRandomString(8) + GenRandomExtension());
            auto detect = (i < testDataHolder.NUnixDetects) ? unixDetectPattern :
                          (i < (testDataHolder.NUnixDetects + testDataHolder.NMacosDetects)) ? macosDetectPattern : "";
            GenRandomFile(filename, testDataHolder.MinFileSize_b, detect);
        }
    }
    
    void checkReport(const fs::path& testDir) {
        const auto scanStat = ScanDirectory(testDir);
        scanStat.Report();
        assert(scanStat.NFilesScanned == testDataHolder.NJsFiles + testDataHolder.NUnixFiles);
        assert(scanStat.NJsDetects == testDataHolder.NJsDetects);
        assert(scanStat.NUnixDetects == testDataHolder.NUnixDetects);
        assert(scanStat.NMacosDetects == testDataHolder.NMacosDetects);
    }
    
    const TestDataHolder testDataHolder = TestDataHolder();
};


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: ./TestSearch <test_dir>" << std::endl;
        return 1;
    }
    const fs::path testDir = argv[1];
    SearchTest().Test(testDir);
}

