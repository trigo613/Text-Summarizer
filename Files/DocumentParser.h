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
    static const std::regex non_standard(R"([^a-zA-Z0-9\s])");
    static const std::regex whitespace_sequence(R"(\s+)");

    vector<string> splitToSentences(const string &document);

    vector<string> splitStringByDelim(const string &s, char delim);

    unordered_set<string> loadWordsWithPeriods();

    void replaceNumbersWithTokenInplace(string &text);

    void removeWordsWithPeriodsInplace(string &text);

    void tokenizeSentenceInplace(vector<string> &sentence);
}


#endif //TEXTSUMMARIZATION_DOCUMENTPARSER_H
