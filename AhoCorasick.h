#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class AhoCorasickSearch {
public:
    AhoCorasickSearch() {
        bor.resize(1);
        bor[0].SuffixNext = 0;
        bor[0].Parent = 0;
    }

    void AddPattern(const std::string& s);
    void Find(const std::string& s);
private:
    struct Vertex {
    public:
        bool IsLeaf = false;
        char Symbol{};
        int Parent = -1;
        int SuffixNext = -1;
        int Shortcut = -1;
        std::unordered_map<char, int> Children;
        std::unordered_map<char, int> TransitionsCache;
//    string Str;
    };

    int moveToNext(int idx, char c);
    int getSuffixNext(int idx);
    int getShortcut(int idx);

    std::vector<Vertex> bor;
};
