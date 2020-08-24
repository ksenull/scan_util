//
// Created by kitno on 24.08.2020.
//

#include "AhoCorasick.h"

#include <iostream>

inline unsigned char char2idx(char c) {
    return c - CHAR_MIN;
}

void AhoCorasickSearch::AddPattern(const std::string& s, int patternId) {
    int curIdx = 0;
    for (char c : s) {
        auto symbolIdx = char2idx(c);
        if (bor[curIdx].Children[symbolIdx] != -1) {
            curIdx = bor[curIdx].Children[symbolIdx];
            continue;
        }
        Vertex tmp;
        tmp.Parent = curIdx;
        tmp.Symbol = c;
        bor.emplace_back(tmp);
        bor[curIdx].Children[symbolIdx] = bor.size() - 1;
        curIdx = bor.size() - 1;
    }
    bor[curIdx].IsLeaf = true;
    bor[curIdx].PatternId = patternId;
}

int AhoCorasickSearch::Find(const std::string& str) {
    int curIdx = 0;
    for (const auto& chr : str) {
        curIdx = moveToNext(curIdx, chr);
        for (int j = curIdx; j > 0; j = getShortcut(j)) {
            if (bor[j].IsLeaf) {
                return bor[j].PatternId;
            }
        }
    }
    return -1;
}

int AhoCorasickSearch::moveToNext(int curIdx, char symbol) {
    auto cur = bor[curIdx];

    auto symbolIdx = char2idx(symbol);
    if (cur.TransitionsCache[symbolIdx] != -1) {
        return cur.TransitionsCache[symbolIdx];
    }

    int transition;
    if (cur.Children[symbolIdx] != -1) {
        transition = cur.Children[symbolIdx];
    }
    else if (curIdx == 0) {
        transition = 0;
    } else {
        transition = moveToNext(getSuffixNext(curIdx), symbol);
    }
    cur.TransitionsCache[symbolIdx] = transition;
    return transition;
}

int AhoCorasickSearch::getSuffixNext(int idx) {
    auto v = bor[idx];
    if (v.SuffixNext != -1) {
        return v.SuffixNext;
    }

    if (idx == 0 || v.Parent == 0)
        v.SuffixNext = 0;
    else
        v.SuffixNext = moveToNext(getSuffixNext(v.Parent), v.Symbol);
    return v.SuffixNext;
}

int AhoCorasickSearch::getShortcut(int curIdx) {
    auto cur = bor[curIdx];
    if (cur.Shortcut != -1) {
        return cur.Shortcut;
    }
    int nextIdx = getSuffixNext(curIdx);
    if (bor[nextIdx].IsLeaf) {
        cur.Shortcut = nextIdx;
    } else if (nextIdx == 0) {
        cur.Shortcut = 0;
    } else {
        cur.Shortcut = getShortcut(nextIdx);
    }
    return cur.Shortcut;
}




