// Copyright 2026 Starostin

#include "textgen.h"
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>

std::vector<std::string> read_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return {};
    }

    std::vector<std::string> words;
    std::string word;
    while (file >> word) {
        words.push_back(word);
    }
    return words;
}

bool save_file(const std::string& filename, const std::string& text) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        return false;
    }

    out << text;
    return true;
}

void create_table(const std::vector<std::string>& words,
    statetab& statetab, prefix& first_prefix) {
    if (words.size() < static_cast<size_t>(NPREF)) {
        return;
    }

    prefix current_prefix;
    for (int i = 0; i < NPREF; ++i) {
        current_prefix.push_back(words[i]);
    }

    first_prefix = current_prefix;
    for (size_t i = NPREF; i < words.size(); ++i) {
        const std::string& suffix = words[i];
        statetab[current_prefix].push_back(suffix);
        current_prefix.pop_front();
        current_prefix.push_back(suffix);
    }
}

std::string generate_text(const statetab& statetab,
    const prefix& first_prefix,
    size_t max_len) {
    if (max_len <= 0) {
        return "";
    }

    std::vector<std::string> words;
    for (const std::string& w : first_prefix) {
        words.push_back(w);
    }

    if (words.empty()) {
        return "";
    }

    prefix current = first_prefix;

    while (words.size() < static_cast<size_t>(max_len)) {
        auto it = statetab.find(current);
        if (it == statetab.end() || it->second.empty()) {
            break;
        }

        const std::vector<std::string>& suffixes = it->second;
        int idx = std::rand() % suffixes.size();
        const std::string& next = suffixes[idx];
        words.push_back(next);

        current.pop_front();
        current.push_back(next);
    }

    std::string text;
    for (size_t i = 0; i < words.size(); ++i) {
        if (i != 0) {
            text += " ";
        }
        text += words[i];
    }
    return text;
}