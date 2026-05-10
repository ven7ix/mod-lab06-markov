// Copyright 2021 GHA Test Team
#include "textgen.h"

#include <gtest/gtest.h>

#include <ctime>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

static bool contains(const std::vector<std::string>& vec,
    const std::string& word) {
    return std::find(vec.begin(), vec.end(), word) != vec.end();
}

// 1
TEST(CreateTableTest, EmptyInput_NoFirstPrefix_NoTable) {
    std::vector<std::string> words = { "одно" };
    statetab stab;
    prefix first_prefix;
    create_table(words, stab, first_prefix);
    EXPECT_TRUE(first_prefix.empty());
    EXPECT_TRUE(stab.empty());
}

// 2
TEST(CreateTableTest, ExactlyNPrefWords_SetsFirstPrefix_EmptyTable) {
    std::vector<std::string> words = { "hello", "world" };
    statetab stab;
    prefix first_prefix;
    create_table(words, stab, first_prefix);
    ASSERT_EQ(first_prefix.size(), 2u);
    EXPECT_EQ(first_prefix[0], "hello");
    EXPECT_EQ(first_prefix[1], "world");
    EXPECT_TRUE(stab.empty());
}

// 3
TEST(CreateTableTest, FirstPrefix_ContainsFirstNPrefWords) {
    std::vector<std::string> words = { "a", "b", "c", "d" };
    statetab stab;
    prefix first_prefix;
    create_table(words, stab, first_prefix);
    ASSERT_EQ(first_prefix.size(), 2u);
    EXPECT_EQ(first_prefix[0], "a");
    EXPECT_EQ(first_prefix[1], "b");
}

// 4
TEST(CreateTableTest, SingleSuffix_StoredUnderPrefix) {
    std::vector<std::string> words = { "a", "b", "c" };
    statetab stab;
    prefix first_prefix;
    create_table(words, stab, first_prefix);
    prefix key = { "a", "b" };
    ASSERT_TRUE(stab.count(key));
    ASSERT_EQ(stab[key].size(), 1u);
    EXPECT_EQ(stab[key][0], "c");
}

// 5
TEST(CreateTableTest, MultipleSuffixes_ForSamePrefix) {
    std::vector<std::string> words = { "a", "b", "c", "a", "b", "d" };
    statetab stab;
    prefix first_prefix;
    create_table(words, stab, first_prefix);
    prefix ab = { "a", "b" };
    ASSERT_TRUE(stab.count(ab));
    const auto& suf = stab[ab];
    EXPECT_EQ(suf.size(), 2u);
    EXPECT_TRUE(contains(suf, "c"));
    EXPECT_TRUE(contains(suf, "d"));
}

// 6
TEST(GenerateTextTest, SingleSuffix_AlwaysChosen) {
    statetab stab;
    prefix key = { "x", "y" };
    stab[key] = { "ONLY" };
    prefix first_prefix = key;
    std::string result = generate_text(stab, first_prefix, 5);
    EXPECT_EQ(result, "x y ONLY");
}

// 7
TEST(GenerateTextTest, MultipleSuffixes_ResultInSet) {
    statetab stab;
    prefix ab = { "a", "b" };
    stab[ab] = { "c", "d", "e" };
    prefix bc = { "b", "c" };
    stab[bc] = { "X" };
    prefix cd = { "c", "d" };
    stab[cd] = { "Y" };
    prefix bd = { "b", "d" };
    stab[bd] = { "Z" };
    prefix be = { "b", "e" };
    stab[be] = { "W" };

    std::srand(42);
    std::string text = generate_text(stab, ab, 20);
    std::vector<std::string> words;
    std::istringstream iss(text);
    std::string w;
    while (iss >> w) {
        words.push_back(w);
    }

    ASSERT_GE(words.size(), 2u);
    EXPECT_EQ(words[0], "a");
    EXPECT_EQ(words[1], "b");

    std::vector<std::string> all_suffixes =
    { "c", "d", "e", "X", "Y", "Z", "W" };
    for (size_t i = 2; i < words.size(); ++i) {
        EXPECT_TRUE(contains(all_suffixes, words[i]))
            << "word \"" << words[i] << "\" not in set";
    }
}

// 8
TEST(GenerateTextTest, RespectsMaxLength) {
    statetab stab;
    prefix ab = { "a", "b" };
    stab[ab] = { "c" };
    prefix bc = { "b", "c" };
    stab[bc] = { "d" };
    prefix cd = { "c", "d" };
    stab[cd] = { "e" };

    std::string result = generate_text(stab, ab, 4);
    std::vector<std::string> words;
    std::istringstream iss(result);
    std::string w;
    while (iss >> w) {
        words.push_back(w);
    }
    EXPECT_EQ(words.size(), 4u);
    EXPECT_EQ(result, "a b c d");
}

// 9
TEST(GenerateTextTest, StopsWhenNoSuffix) {
    statetab stab;
    prefix start = { "x", "y" };
    stab[start] = { "z" };
    std::string result = generate_text(stab, start, 10);
    EXPECT_EQ(result, "x y z");
}

// 10
TEST(GenerateTextTest, MaxLenZero_EmptyString) {
    statetab stab;
    prefix start = { "a", "b" };
    stab[start] = { "c" };
    std::string result = generate_text(stab, start, 0);
    EXPECT_TRUE(result.empty());
}

// 11
TEST(GenerateTextTest, FirstWordsEqualFirstPrefix) {
    statetab stab;
    prefix start = { "one", "two" };
    stab[start] = { "three", "four" };
    prefix two_three = { "two", "three" };
    stab[two_three] = { "five" };
    std::string result = generate_text(stab, start, 5);
    EXPECT_EQ(result.substr(0, 7), "one two");
}