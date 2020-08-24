#pragma once

#include <climits>
#include <string>
#include <unordered_map>
#include <vector>

static const unsigned CharRange = CHAR_MAX - CHAR_MIN;

class AhoCorasickSearch {
public:
    AhoCorasickSearch() {
        bor.resize(1);
        bor[0].SuffixNext = 0;
        bor[0].Parent = 0;
    }

    void AddPattern(const std::string& s, int patternId);
    int Find(const std::string& s);
private:
    struct Vertex {
    public:
        bool IsLeaf = false;
        char Symbol{};
        int PatternId = -1;
        int Parent = -1;
        int SuffixNext = -1;
        int Shortcut = -1;
        int Children[CharRange];
        int TransitionsCache[CharRange];

        Vertex() {
            memset(&Children, -1,  CharRange * sizeof(int));
            memset(&TransitionsCache, -1, CharRange * sizeof(int));
        }
    };

    int moveToNext(int idx, char symbol);
    int getSuffixNext(int idx);
    int getShortcut(int idx);

    std::vector<Vertex> bor;
};
