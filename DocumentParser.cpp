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

std::string DocumentParser::removePeriodFromWords(const std::string& text) {

    std::unordered_set<std::string> words_with_periods = loadWordsWithPeriods("words_with_periods.txt");
    std::string result = text;
    // Replace each word with its period removed
    for (const auto& word : words_with_periods) {
        std::string word_without_period = word;
        word_without_period.erase(std::remove(word_without_period.begin(), word_without_period.end(), '.'), word_without_period.end());
        // Find and replace all occurrences in the result string
        size_t pos = 0;
        while ((pos = result.find(word, pos)) != std::string::npos) {
            result.replace(pos, word.length(), word_without_period);
            pos += word_without_period.length(); // Move past the newly replaced word
        }
    }
    return result;
}

Document DocumentParser::splitToSentences (const std::string &document) {
    return splitString(document,'.');
}

Sentence DocumentParser::splitSentenceToWords(const std::string& sentence, bool erase_extras) {
    return splitString(sentence,' ', erase_extras);
}

Sentence DocumentParser::splitString(const std::string& s, char delim, bool erase_extras) {
    std::vector<std::string> result;
    std::istringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        if (erase_extras) {
            trimString(item);
        }
        if (!item.empty()) {
            result.push_back(std::move(item));
        }
    }
    return result;
}


SentenceList DocumentParser::splitDocumentToSentences(const std::string& document,bool erase_extras) {
    Document doc = splitToSentences(document);
    cleanDocument(doc);
    SentenceList out;
    out.reserve(doc.size());
    for(const auto & i : doc){
        out.push_back(splitSentenceToWords(i,erase_extras));
    }
    return out;
}

void DocumentParser::trimString(std::string& s) {
    const std::string charsToTrim = " \t\n\r\v\f,\'`-!?;:()[]{}<>\"“”#$%^&*_+=|\\/~";
    s.erase(0, s.find_first_not_of(charsToTrim));       // Trim leading characters
    s.erase(s.find_last_not_of(charsToTrim) + 1);       // Trim trailing characters
}

void DocumentParser::cleanDocument(Document &d) {
    for (auto &s : d) {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        removeNonStandardChars(s);
        replaceConsecutiveWhitespaces(s);
    }
}

std::string DocumentParser::removeWordsWithDots(const std::string& text) {
    // Regex pattern to match words with dots before and after them
    std::regex pattern(R"(\.\S+\.?)");
    return std::regex_replace(text, pattern, "");
}

//write a function like the one above that modifies the string itself instead of returning a new string
void DocumentParser::removeNonStandardChars(std::string &text) {
    std::regex pattern(R"([^a-zA-Z0-9\s])");
    text = std::regex_replace(text, pattern, " ");
}

#include <string>
#include <regex>

void DocumentParser::replaceConsecutiveWhitespaces(std::string& text) {
    std::regex pattern(R"(\s+)");
    text = std::regex_replace(text, pattern, " ");
}
