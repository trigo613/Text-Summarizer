//
// Created by skche on 01/10/2024.
//

#include "TextRank.h"
#include "Constants.h"


double TextRank::cosine_similarity(const SparseVector &a, const SparseVector &b) {
    return dot(a, b) / (a.norm() * b.norm());
}


SimilarityMatrix TextRank::generateSimilarityMatrix(const SentenceEmbeddingList &embeddings) {
    size_t N = embeddings.size();
    SimilarityMatrix similarityMatrix(N, std::vector<double>(N, 0.0));

    for (size_t i = 0; i < N; i++) {
        if (i % 100 == 0)
            std::cout << "Calculating similarity for sentence " << i  << " of " << N << std::endl;
        for (size_t j = i + 1; j < N; j++) {
            similarityMatrix[i][j] = cosine_similarity(embeddings[i], embeddings[j]);
            similarityMatrix[j][i] = similarityMatrix[i][j];
        }
    }
    for (size_t i = 0; i < N; i++) {
        similarityMatrix[i][i] = 1.0;
    }
    return similarityMatrix;
}


std::vector<int> TextRank::textRank(const SimilarityMatrix &similarityMatrix, int maxIter, double d) {
    size_t n = similarityMatrix.size();
    std::vector<double> scores(n, 1.0);
    std::vector<double> tempScores(n, 0.0);
    std::vector<double> similaritySums(n, 0.0);
    for (size_t j = 0; j < n; ++j) {
        similaritySums[j] = std::accumulate(similarityMatrix[j].begin(),
                                            similarityMatrix[j].end(), 0.0);
    }
    for (int iter = 0; iter < maxIter; ++iter) {
        bool converged = true;
        for (size_t i = 0; i < n; ++i) {
            tempScores[i] = (1 - d);
            for (size_t j = 0; j < n; ++j) {
                if (i != j && similaritySums[j] > 0) {
                    tempScores[i] += d * (similarityMatrix[j][i] / similaritySums[j]) * scores[j];
                }
            }
            // Check for convergence
            if (std::fabs(tempScores[i] - scores[i]) > MIN_THRESHOLD) {
                converged = false; // If any score changes beyond the threshold, mark as not converged
            }
        }
        scores = tempScores;

        if (converged) {
            break;
        }
    }

    std::vector<int> ranks(n);
    for (int i = 0; i < n; ++i) {
        ranks[i] = i;
    }
    std::sort(ranks.begin(), ranks.end(), [&scores](int a, int b) {
        return scores[a] > scores[b];
    });
    return ranks;
}


