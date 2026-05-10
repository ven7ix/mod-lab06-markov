// Copyright 2026 Andrew

#ifndef INCLUDE_TEXTGEN_H_
#define INCLUDE_TEXTGEN_H_

#include <deque>
#include <map>
#include <vector>
#include <string>
#include <format>

typedef std::deque<std::string> prefix;
typedef std::map<prefix, std::vector<std::string>> statetab;

const size_t NPREF = 2;
const size_t MAXGEN = 1000;

std::vector<std::string> read_file(
    const std::string& filename);

bool save_file(
    const std::string& filename, const std::string& text);

void create_table(
    const std::vector<std::string>& words,
    statetab& statetab,
    prefix& first_prefix);

std::string generate_text(
    const statetab& statetab,
    const prefix& first_prefix,
    size_t max_len);

#endif //INCLUDE_TEXTGEN_H_