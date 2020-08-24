cmake_minimum_required(VERSION 3.15)
project(scan_util)

set(CMAKE_CXX_STANDARD 17)

add_executable(scan_util main.cpp DirectoryScanner.cpp DirectoryScanner.h FileScanner.cpp FileScanner.h Detect.cpp Detect.h AhoCorasick.cpp AhoCorasick.h)