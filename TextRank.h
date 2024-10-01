//
// Created by skche on 01/10/2024.
//

#ifndef TEXT_SUMMARIZER_TEXTRANK_H
#define TEXT_SUMMARIZER_TEXTRANK_H

#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <stdexcept>
#include <cmath>
#include "Constants.h"

namespace TextRank {

    SimilarityMatrix generateSimilarityMatrix(const SentenceEmbeddingList &embeddings);

    std::vector<int> textRank(const SimilarityMatrix &similarityMatrix, int maxIter, double d);

    double cosine_similarity(SparseVector a, SparseVector b);
};


#endif //TEXT_SUMMARIZER_TEXTRANK_H
