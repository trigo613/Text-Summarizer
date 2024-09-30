//
// Created by skche on 29/09/2024.
//

#ifndef TEXTSUMMARIZATION_TFIDF_H
#define TEXTSUMMARIZATION_TFIDF_H

#include "Constants.h"
#include <cmath>
#include <iostream>
#include <numeric>

class TfIdf {
    static SentenceEmbeddingList
    generateCountVectors(const SentenceList &sentences, const WordFrequencyCounter &word_freq_counter,
                         const WordToInt &word_to_index);

    static WordFrequencyCounter generateDocumentWordCountMap(const SentenceList &sentences);

    static WordToInt generateWordToIndex(const WordFrequencyCounter& word_freq_counter);

    static SentenceEmbeddingList normalizeCountVectors(SentenceEmbeddingList &embedding_list);

    static Idf generateIdf(const SentenceEmbeddingList &wordCounts);
public:


    static SentenceEmbeddingList generateEmbeddings(const SentenceList &sentences);


};


#endif //TEXTSUMMARIZATION_TFIDF_H
