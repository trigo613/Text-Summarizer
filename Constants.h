//
// Created by skche on 29/09/2024.
//

#ifndef TEXTSUMMARIZATION_CONSTANTS_H
#define TEXTSUMMARIZATION_CONSTANTS_H

#include <vector>
#include <string>
#include <map>

typedef std::vector<std::vector<double>> SimilarityMatrix;

typedef std::vector<std::string> Document;

typedef std::vector<std::string> Sentence;

typedef std::vector<Sentence> SentenceList;

typedef std::vector<double> SentenceEmbedding;

typedef std::vector<SentenceEmbedding> SentenceEmbeddingList;

typedef std::map<std::string,std::vector<int>> WordFrequencyCounter;

typedef std::map<std::string,int> WordToInt;

typedef std::vector<double> Idf;



#endif //TEXTSUMMARIZATION_CONSTANTS_H
