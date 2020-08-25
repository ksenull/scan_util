#include <iostream>

#include "scan/DirectoryStats.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: ./scan_util <dirpath>" << std::endl;
    }

    const std::string path = argv[1];

    auto scanStat = ScanDirectory(path);
    scanStat.Report();

    return 0;
}

