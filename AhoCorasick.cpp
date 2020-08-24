//
// Created by kitno on 24.08.2020.
//

#include "AhoCorasick.h"

#include <iostream>


void AhoCorasickSearch::AddPattern(const std::string& s) {
    int curIdx = 0;
    for (char c : s) {
        auto&& v = bor[curIdx];
        if (v.Children[c] == -1) {
            Vertex tmp;
            tmp.Parent = curIdx;
            tmp.Symbol = c;
            v.Children[c] = bor.size();
            bor.emplace_back(tmp);
        }
        curIdx = v.Children[c];
    }
    bor[curIdx].IsLeaf = true;
//    bor[curIdx].str = s;
}

void AhoCorasickSearch::Find(const std::string& s) {
    int curIdx = 0;
    for (auto i = 0U; i < s.length(); i++) {
        curIdx = moveToNext(curIdx, s[i]);
//        cout << "Move to " << bor[curIdx].str << " with " << s[i] << endl;
        for (int j = curIdx; j > 0; j = getShortcut(j)) {
            if (bor[j].IsLeaf) {
                std::cout << "Found pattern: " << std::endl; //<< bor[j].str;
                return;
            }
        }
    }
}

int AhoCorasickSearch::moveToNext(int curIdx, char c) {
    auto cur = bor[curIdx];

    auto transitionIt = cur.TransitionsCache.find(c);
    if (transitionIt != cur.TransitionsCache.end()) {
        return transitionIt->second;
    }

    int transition;
    auto next = cur.Children.find(c);
    if (next != cur.Children.end()) {
        transition = next->second;
    }
    else if (curIdx == 0) {
        transition = 0;
    } else {
        transition = moveToNext(getSuffixNext(curIdx), c);
    }
    cur.TransitionsCache[c] = transition;
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
    auto next = bor[nextIdx];
    if (bor[nextIdx].IsLeaf) {
        cur.Shortcut = nextIdx;
    } else if (nextIdx == 0) {
        cur.Shortcut = 0;
    } else {
        cur.Shortcut = getShortcut(nextIdx);
    }
    return cur.Shortcut;
}




