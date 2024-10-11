//
// Created by skche on 29/09/2024.
//

#ifndef TEXTSUMMARIZATION_CONSTANTS_H
#define TEXTSUMMARIZATION_CONSTANTS_H

#include <vector>
#include <string>
#include <map>
#include "SparseVector.h"

#define MIN_THRESHOLD 0.00001
#define MIN_WORDS_PER_SENTENCE 4

typedef std::vector<std::vector<double>> SimilarityMatrix;

typedef std::vector<std::string> StringVector;

typedef std::vector<StringVector> SentenceList;

typedef SparseVector SentenceEmbedding;

typedef std::vector<SentenceEmbedding> SentenceEmbeddingList;

typedef std::map<std::string,std::vector<int>> WordFrequencyCounter;

typedef std::map<std::string,int> WordToInt;

typedef std::map<std::string,int> IntToWord;

typedef std::vector<double> Idf;



#endif //TEXTSUMMARIZATION_CONSTANTS_H
