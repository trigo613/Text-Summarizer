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

    vector<vector<double>> generateSimilarityMatrix(vector<SparseVector> &embeddings);

    vector<int> textRank(const vector<vector<double>> &similarityMatrix, int maxIter, double d);

    double cosine_similarity(SparseVector& a, SparseVector& b);
}


#endif //TEXT_SUMMARIZER_TEXTRANK_H
