//
// Created by skche on 29/09/2024.
//

#include "DocumentParser.h"
#include "Constants.h"

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

void DocumentParser::removeWordsWithPeriodsInplace(std::string &text) {
    std::unordered_set<std::string> words_with_periods = loadWordsWithPeriods();

    std::ostringstream modifiedText;
    std::istringstream inputStream(text);
    std::string word;
    bool firstWord = true;

    while (inputStream >> word) {
        if (word.find('.') != std::string::npos) {  // Check if the word contains a period
            if (words_with_periods.find(word) != words_with_periods.end()) {
                word.erase(std::remove(word.begin(), word.end(), '.'), word.end());
            } else if (std::count(word.begin(), word.end(), '.') > 1) {
                continue; // Skip words with multiple periods
            }
        }

        if (!firstWord) {
            modifiedText << " ";
        }
        modifiedText << word;
        firstWord = false;
    }

    text = modifiedText.str();
}

vector<string> DocumentParser::splitToSentences(const string &document) {
    vector<string> result = splitStringByDelim(document, SENTENCE_DELIMETER);
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