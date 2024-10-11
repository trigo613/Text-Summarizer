//
// Created by skche on 29/09/2024.
//

#ifndef TEXTSUMMARIZATION_DOCUMENTPARSER_H
#define TEXTSUMMARIZATION_DOCUMENTPARSER_H

#include <unordered_set>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <fstream>
#include <regex>
#include "Constants.h"
#include <string>
#include <regex>

namespace DocumentParser {

     StringVector splitString(const std::string& s, char delim, bool clean_string = true);

     StringVector splitSentenceToWords(const std::string& sentence, bool clean_string = true);

     void keepAlphanumericOnly(std::string &s);

     void replaceNumbersWithString(std::string &s);

     std::unordered_set<std::string> loadWordsWithPeriods(const std::string& file_path);

     StringVector splitToSentences(const std::string& document, bool clean_string = true);

     std::string removeWordsWithPeriods(const std::string &text);

     StringVector cleanDocument(const StringVector &d);

    std::string removeNonStandardChars(const std::string &text);

    std::string replaceConsecutiveWhitespaces(const std::string &text);

    SentenceList splitSentencesToWords(const StringVector& document, bool clean_string);
};


#endif //TEXTSUMMARIZATION_DOCUMENTPARSER_H
