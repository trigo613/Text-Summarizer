//
// Created by skche on 29/09/2024.
//


#include "TfIdf.h"

WordFrequencyCounter TfIdf::generateDocumentWordCountMap(const SentenceList& sentences) {
    WordFrequencyCounter map;
    for(int i = 0 ; i < sentences.size(); i++){
        Sentence  sentence = sentences[i];
        for(const auto& word : sentence){
            if (map.find(word) == map.end()) {
                map.insert({word,std::vector<int>(sentences.size(),0)});
                map[word][i] = 1;
            }
            else{
                map[word][i] += 1;
            }
        }
    }
    return map;
}

SentenceEmbeddingList TfIdf::generateEmbeddings(const SentenceList& sentences) {
    WordFrequencyCounter map = TfIdf::generateDocumentWordCountMap(sentences);
    WordToInt word_to_index = TfIdf::generateWordToIndex(map);
    SentenceEmbeddingList countVectors = TfIdf::generateCountVectors(sentences,map,word_to_index);
    SentenceEmbeddingList normalized_embeddings = TfIdf::normalizeCountVectors(countVectors);
    Idf idf = TfIdf::generateIdf(countVectors);
    for(int i = 0; i < sentences.size(); i ++){
        for(int j =0; j < word_to_index.size(); j ++){
            normalized_embeddings[i][j] = normalized_embeddings[i][j] * idf[j];
        }
    }
    return normalized_embeddings;
}

Idf TfIdf::generateIdf(const SentenceEmbeddingList& wordCounts){
    Idf idf = Idf(wordCounts[0].size(),0);
    for(auto embedding : wordCounts){
        for(int i = 0 ; i < embedding.size(); i ++){
            if(embedding[i] != 0){
                idf[i] += 1;
            }
        }
    }
    auto numSentences = (double)wordCounts.size();
    for(double & i : idf){
        i = log(numSentences/i) + 1;
    }
    return idf;
}

WordToInt TfIdf::generateWordToIndex(const WordFrequencyCounter& word_freq_counter){
    WordToInt map{};
    int counter = 0;
    for (const auto& pair : word_freq_counter) {
        map.insert({pair.first,counter++});
    }
    return map;
}

SentenceEmbeddingList TfIdf::generateCountVectors(const SentenceList& sentences,
                                                  const WordFrequencyCounter& word_freq_counter,
                                                  const WordToInt& word_to_index) {
    size_t embeddingSize = word_to_index.size();
    SentenceEmbeddingList embeddings(sentences.size());
    for(int i = 0; i < sentences.size(); i++){
        SentenceEmbedding embedding(embeddingSize,0);
        for (const auto& word : sentences[i]){
            const int word_index = word_to_index.at(word);
            const int word_count = word_freq_counter.at(word)[i];
            embedding[word_index] = word_count;
        }
        embeddings[i] = embedding;
    }
    return embeddings;
}

SentenceEmbeddingList
TfIdf::normalizeCountVectors(SentenceEmbeddingList& embedding_list) {
    size_t embeddingSize = embedding_list[0].size();
    for(auto & i : embedding_list){
        double sum = std::accumulate(i.begin(),i.end(),0.0);
        for(int j = 0 ; j < embeddingSize; j++){
            i[j] = i[j]/sum;
        }
    }
    return embedding_list;
}
