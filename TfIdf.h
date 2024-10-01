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

private:
    WordFrequencyCounter word_freq_counter;
    WordToInt word_to_index;
    IntToWord index_to_word;
    Idf idf;
    size_t num_sentences;
    size_t embedding_size;

    SentenceEmbeddingList generateCountVectors(const SentenceList &sentences);

    void generateDocumentWordCountMap(const SentenceList &sentences);

    void generateWordToIndex();

    SentenceEmbeddingList normalizeCountVectors(SentenceEmbeddingList &embedding_list);

    void generateIdf();

public:

    void fit(const SentenceList &sentences);

    SentenceEmbeddingList transform(const SentenceList &sentences);

};


#endif //TEXTSUMMARIZATION_TFIDF_H
