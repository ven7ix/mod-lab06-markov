// Copyright 2026 Andrew

#include <ctime>
#include <iostream>
#include <vector>
#include <string>
#include "textgen.h"

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    std::string inputFile = "input.txt";
    std::vector<std::string> words = read_file(inputFile);

    if (words.size() < NPREF) {
        std::cout << "Not enough words" << std::endl;

        std::string originalText;
        for (size_t i = 0; i < words.size(); ++i) {
            if (i != 0) {
                originalText += " ";
            }
            originalText += words[i];
        }

        save_file("gen.txt", originalText);
        return 0;
    }

    statetab stab;
    prefix firstPrefix;
    create_table(words, stab, firstPrefix);
    std::string generated = generate_text(stab, firstPrefix, MAXGEN);

    if (save_file("gen.txt", generated)) {
        std::cout << "Text is saved in gen.txt" << std::endl;
    } else {
        std::cerr << "Error writing to gen.txt" << std::endl;
    }

    return 0;
}
