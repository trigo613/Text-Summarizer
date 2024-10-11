//
// Created by skche on 29/09/2024.
//


#include "TfIdf.h"

void TfIdf::generateDocumentWordCountMap(const SentenceList &sentences) {
    for (int i = 0; i < sentences.size(); i++) {
        StringVector sentence = sentences[i];
        for (const auto &word: sentence) {
            if (word_freq_counter.find(word) == word_freq_counter.end()) {
                word_freq_counter.insert({word, std::vector<int>(sentences.size(), 0)});
                word_freq_counter[word][i] = 1;
            } else {
                word_freq_counter[word][i] += 1;
            }
        }
    }
}


void TfIdf::generateIdf() {
    idf = Idf(embedding_size, 0);
    int count = 0;
    for (auto pair: word_freq_counter) {
        for (int i = 0; i < num_sentences; i++) {
            if (pair.second[i] != 0) {
                idf[count] += 1;
            }
        }
        idf[count] = log((double) num_sentences + 1/ idf[count] + 1);
        count++;
    }
}

void TfIdf::generateWordToIndex() {
    int counter = 0;
    for (const auto &pair: word_freq_counter) {
        word_to_index.insert({pair.first, counter++});
    }
}

SentenceEmbeddingList TfIdf::generateCountVectors(const SentenceList &sentences) {
    size_t embeddingSize = word_to_index.size();
    SentenceEmbeddingList embeddings(sentences.size(), SentenceEmbedding((int)embeddingSize, 0));
    for (int i = 0; i < sentences.size(); i++) {
        for (const auto &word: sentences[i]) {
            const int word_index = word_to_index.at(word);
            const int word_count = word_freq_counter.at(word)[i];
            embeddings[i].insert(word_index, word_count);

        }
    }
    return embeddings;
}

SentenceEmbeddingList TfIdf::normalizeCountVectors(SentenceEmbeddingList &embedding_list) {
    size_t embeddingSize = embedding_list[0].size();
    for (auto &i: embedding_list) {
        double sum = i.sum();
        for (int j = 0; j < embeddingSize; j++) {
            i.insert(j, i[j] / sum);
        }
    }
    return embedding_list;
}

void TfIdf::fit(const SentenceList &sentences) {
    num_sentences = sentences.size();

    generateDocumentWordCountMap(sentences);

    generateWordToIndex();

    embedding_size = word_to_index.size();

    generateIdf();

}

SentenceEmbeddingList TfIdf::transform(const SentenceList &sentences) {
    SentenceEmbeddingList countVectors = generateCountVectors(sentences);
    SentenceEmbeddingList normalized_embeddings = normalizeCountVectors(countVectors);
    for (int i = 0; i < sentences.size(); i++) {
        for (int j = 0; j < word_to_index.size(); j++) {
            normalized_embeddings[i].insert(j, normalized_embeddings[i][j] * idf[j]);
        }
    }
    return normalized_embeddings;
}

