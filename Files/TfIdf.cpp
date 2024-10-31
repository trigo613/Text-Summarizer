//
// Created by skche on 29/09/2024.
//


#include "TfIdf.h"

void TfIdf::generateDocumentWordCountMap(const vector<vector<string>> &sentences) {
    for (int i = 0; i < sentences.size(); i++) {
        vector<string> sentence = sentences[i];
        for (const auto &word: sentence) {
            if (word_freq_counter.find(word) == word_freq_counter.end()) {
                word_freq_counter.insert({word, vector<int>(sentences.size(), 0)});
                word_freq_counter[word][i] = 1;
            } else {
                word_freq_counter[word][i] += 1;
            }
        }
    }
}


void TfIdf::generateIdf() {
    idf = vector<double>(embedding_size, 0);
    int count = 0;
    for (auto pair: word_freq_counter) {
        for (int i = 0; i < num_sentences; i++) {
            if (pair.second[i] != 0) {
                idf[count] += 1;
            }
        }
        idf[count] = log((double)(num_sentences + 1) / (idf[count] + 1));
        count++;
    }
}

void TfIdf::generateWordToIndex() {
    int counter = 0;
    for (const auto &pair: word_freq_counter) {
        word_to_index.insert({pair.first, counter++});
    }
}

vector<SparseVector> TfIdf::generateCountVectors(const vector<vector<string>> &sentences) const{
    size_t embeddingSize = word_to_index.size();
    vector<SparseVector> embeddings(sentences.size(), SparseVector((int)embeddingSize, 0));
    for (int i = 0; i < sentences.size(); i++) {
        for (const auto &word: sentences[i]) {
            int word_index = word_to_index.at(word);
            int word_count = word_freq_counter.at(word)[i];

            embeddings[i].insert(word_index, word_count);
        }
    }
    return embeddings;
}



vector<SparseVector> TfIdf::normalizeCountVectors(vector<SparseVector> &embedding_list) const {
    vector<SparseVector> normalizedVectors;
    for (int i = 0; i < embedding_list.size(); i++) {
        double norm = embedding_list[i].norm();
        SparseVector normalizedVector(embedding_list[i].size());
        if (norm != 0) {
            for (int j = 0; j < embedding_list[i].size(); j++) {
                normalizedVector.insert(j, embedding_list[i][j] / norm);
            }
        }
        normalizedVectors.push_back(normalizedVector);
    }
    return normalizedVectors;
}

void TfIdf::fit(const vector<vector<string>> &sentences) {
    this->num_sentences = sentences.size();

    generateDocumentWordCountMap(sentences);

    generateWordToIndex();

    this->embedding_size = word_to_index.size();

    generateIdf();

}

vector<SparseVector> TfIdf::transform(vector<vector<string>> &sentences) const{
    vector<SparseVector> countVectors = generateCountVectors(sentences);
    vector<SparseVector> normalized_vectors = normalizeCountVectors(countVectors);
    for (int i = 0; i < sentences.size(); i++) {
        for (int j = 0; j < word_to_index.size(); j++) {
            normalized_vectors[i].insert(j, normalized_vectors[i][j] * idf[j]);
        }
    }
    return normalized_vectors;
}



