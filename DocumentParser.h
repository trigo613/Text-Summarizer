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

class DocumentParser {
    static Sentence splitString(const std::string& s,char delim,bool erase_extras = true);

    static Sentence splitSentenceToWords(const std::string& sentence,bool erase_extras = true);
    static void trimString(std::string &s);

    static std::unordered_set<std::string> loadWordsWithPeriods(const std::string& file_path);

public:
    static SentenceList splitDocumentToSentences(const std::string& document,bool erase_extras = true);

    static Document splitToSentences(const std::string& document);

    static std::string removePeriodFromWords(const std::string &text);

    static std::string removeWordsWithDots(const std::string &text);

    static void cleanDocument(Document &d);

    static void removeNonStandardChars(std::string &text);

    static void replaceConsecutiveWhitespaces(std::string &text);
};


#endif //TEXTSUMMARIZATION_DOCUMENTPARSER_H
