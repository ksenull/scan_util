#include <iostream>

#include "DirectoryScanner.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: ./scan_util <dirpath>" << std::endl;
    }

    const std::string path = argv[1];

    DirectoryScanner scanner(path);
    scanner.Scan();
    scanner.PrintReport();

    return 0;
}

