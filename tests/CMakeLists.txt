add_test(TestSearch scan_util_test)

set(PROJECT_FILES
        ../scan/DirectoryStats.cpp ../scan/DirectoryStats.h
        ../scan/FileScanner.cpp ../scan/FileScanner.h
        ../scan/Detect.h
        ../scan/search/AhoCorasick.cpp ../scan/search/AhoCorasick.h)

add_executable(TestSearch TestSearch.cpp ${PROJECT_FILES})