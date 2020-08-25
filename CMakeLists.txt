cmake_minimum_required(VERSION 3.15)
project(scan_util)

set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

add_executable(scan_util main.cpp scan/DirectoryStats.cpp scan/DirectoryStats.h scan/FileScanner.cpp scan/FileScanner.h scan/Detect.h scan/search/AhoCorasick.cpp scan/search/AhoCorasick.h)