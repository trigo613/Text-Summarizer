//
// Created by skche on 29/09/2024.
//

#include "DocumentParser.h"


std::unordered_set<std::string> DocumentParser::loadWordsWithPeriods(const std::string& file_path) {
    std::unordered_set<std::string> words;
    std::ifstream file(file_path);
    std::string word;
    // Read each word from the file and insert into the set
    while (std::getline(file, word)) {
        words.insert(word);
    }
    return words;
}

std::string DocumentParser::removeWordsWithPeriods(const std::string& text) {

    std::unordered_set<std::string> words_with_periods = loadWordsWithPeriods("words_with_periods.txt");
    std::string result = text;
    // Replace each word with its period removed
    for (const auto& word : words_with_periods) {
        std::string word_without_period = word;
        word_without_period.erase(std::remove(word_without_period.begin(),
                                                    word_without_period.end(), '.'),
                                                word_without_period.end());
        // Find and replace all occurrences in the result string
        size_t pos = 0;
        while ((pos = result.find(word, pos)) != std::string::npos) {
            result.replace(pos, word.length(), word_without_period);
            pos += word_without_period.length();
        }
    }
    // Remove any words with two or more dots
    std::regex pattern(R"(\b\w*\.\w*\.\w*\b)");
    return std::regex_replace(result, pattern, "");
}

StringVector DocumentParser::splitToSentences (const std::string &document, bool clean_string) {
       return splitString(document,'.',clean_string);
}

StringVector DocumentParser::splitSentenceToWords(const std::string& sentence, bool clean_string) {
    return splitString(sentence,' ', clean_string);
}

StringVector DocumentParser::splitString(const std::string& s, char delim, bool clean_string) {
    std::vector<std::string> result;
    std::istringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        if (clean_string) {
            keepAlphanumericOnly(item);
            std::transform(item.begin(), item.end(), item.begin(), ::tolower);
            replaceNumbersWithString(item);
        }
        if (!item.empty()) {
            result.push_back(std::move(item));
        }
    }
    return result;
}


SentenceList DocumentParser::splitSentencesToWords(const StringVector& document,bool clean_string) {
    StringVector clean_document = cleanDocument(document);
    SentenceList out;
    out.reserve(document.size());
    for(const auto & i : document){
        out.push_back(splitSentenceToWords(i, clean_string));
    }
    return out;
}

void DocumentParser::keepAlphanumericOnly(std::string& s) {
    s.erase(std::remove_if(s.begin(), s.end(), [](char c) {
        return !std::isalnum(c) && c != '.' && c != ' ';
    }), s.end());
}

void DocumentParser::replaceNumbersWithString(std::string& s) {
    std::string result;
    size_t length = s.length();

    for (size_t i = 0; i < length; ++i) {
        if (std::isdigit(s[i])) {
            size_t start = i;
            while (i < length && std::isdigit(s[i])) {
                ++i;
            }
            // Number length
            size_t num_length = i - start;
            // Replace with _DIGITS format
            result += std::to_string(num_length) + "_DIGITS";
            --i; // To adjust the loop after the inner while
        } else {
            result += s[i];
        }
    }

    s = result;
}


StringVector DocumentParser::cleanDocument(const StringVector &d) {
    StringVector out;
    out.reserve(d.size());
    for(const auto & i : d){
        std::string temp = i;
        temp = removeNonStandardChars(temp);
        temp = replaceConsecutiveWhitespaces(temp);
        out.push_back(temp);
    }
    return out;
}

//write a function like the one above that modifies the string itself instead of returning a new string
std::string DocumentParser::removeNonStandardChars(const std::string &text) {
    std::regex pattern(R"([^a-zA-Z0-9\s])");
    return std::regex_replace(text, pattern, " ");
}

std::string DocumentParser::replaceConsecutiveWhitespaces(const std::string& text) {
    std::regex pattern(R"(\s+)");
    return std::regex_replace(text, pattern, " ");
}
