//
// Created by skche on 29/09/2024.
//

#include "DocumentParser.h"

#include <iostream>

unordered_set<string> DocumentParser::loadWordsWithPeriods() {
    unordered_set<string> words;
    string file_path = "../words_with_periods.txt";
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << file_path << std::endl;
        exit(1);
    }
    string word;
    // Read each word from the file and insert into the set
    while (std::getline(file, word)) {
        words.insert(word);
    }
    file.close();
    return words;
}

void DocumentParser::removeWordsWithPeriodsInplace(string &text) {

    unordered_set<string> words_with_periods = loadWordsWithPeriods();
    // Replace each word with its period removed
    for (const auto &word: words_with_periods) {
        string word_without_period = word;
        word_without_period.erase(std::remove(word_without_period.begin(),
                                              word_without_period.end(), '.'),
                                  word_without_period.end());
        // Find and replace all occurrences in the result string
        size_t pos = 0;
        while ((pos = text.find(word, pos)) != string::npos) {
            text.replace(pos, word.length(), word_without_period);
            pos += word_without_period.length();
        }
    }
    // Remove any words with two or more dots
    std::regex pattern(R"(\b\w*\.\w*\.\w*\b)");
    text = std::regex_replace(text, pattern, "");
}

vector<string> DocumentParser::splitToSentences(const string &document) {
    vector<string> result = splitStringByDelim(document, '.');
    vector<string> out;
    out.reserve(result.size());
    for (const auto &i : result) {
        if (i.size() >= MIN_WORDS_PER_SENTENCE) {
            out.push_back(i);
        }
    }
    out.shrink_to_fit();
    return out;
}

vector<string> DocumentParser::splitStringByDelim(const string &s, char delim) {
    vector<string> result;
    std::istringstream ss(s);
    string item;
    while (std::getline(ss, item, delim)) {
        if (!item.empty()) {
            result.push_back(std::move(item));
        }
    }
    result.shrink_to_fit();
    return result;
}

void DocumentParser::replaceNumbersWithTokenInplace(string &s) {
    string result;
    size_t length = s.length();
    for (size_t i = 0; i < length; ++i) {
        if (std::isdigit(s[i])) {
            size_t start = i;
            while (i < length && std::isdigit(s[i])) {
                ++i;
            }
            size_t num_length = i - start;
            result += std::to_string(num_length) + "_DIGITS";
            --i;
        } else {
            result += s[i];
        }
    }

    s = result;
}

void DocumentParser::tokenizeSentenceInplace(vector<string> &sentence){
    for (auto &i: sentence) {
        std::transform(i.begin(), i.end(), i.begin(), ::tolower);
        i = std::regex_replace(i, non_standard, "");
        i = std::regex_replace(i, whitespace_sequence, " ");
        replaceNumbersWithTokenInplace(i);
    }
}