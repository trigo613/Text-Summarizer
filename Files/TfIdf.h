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
    map<string,vector<int>> word_freq_counter;
    map<string,int> word_to_index;
    map<int,string> index_to_word;
    vector<double> idf;
    size_t num_sentences;
    size_t embedding_size;

    vector<SparseVector> generateCountVectors(const vector<vector<string>> &sentences) const;

    void generateDocumentWordCountMap(const vector<vector<string>> &sentences);

    void generateWordToIndex();

    vector<SparseVector> normalizeCountVectors(vector<SparseVector> &embedding_list) const;

    void generateIdf();

public:

    void fit(const vector<vector<string>> &sentences);

    vector<SparseVector> transform(vector<vector<string>> &sentences) const;

};


#endif //TEXTSUMMARIZATION_TFIDF_H
