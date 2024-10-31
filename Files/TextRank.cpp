#include "TextRank.h"
#include "Constants.h"

double TextRank::cosine_similarity(SparseVector& a,SparseVector& b) {
    return dot(a, b) / (a.norm() * b.norm());
}

vector<vector<double>> TextRank::generateSimilarityMatrix(vector<SparseVector> &embeddings) {
    size_t N = embeddings.size();
    vector<vector<double>> similarityMatrix(N, vector<double>(N, 0.0));

    for (size_t i = 0; i < N; ++i) {
        for (size_t j = i + 1; j < N; ++j) {
            double similarity = cosine_similarity(embeddings[i], embeddings[j]);
            similarityMatrix[i][j] = similarity;
            similarityMatrix[j][i] = similarity;
        }
    }

    for (size_t i = 0; i < N; i++) {
        similarityMatrix[i][i] = 1.0;
    }

    return similarityMatrix;
}

vector<int> TextRank::textRank(const vector<vector<double>> &similarityMatrix, int maxIter, double d) {
    size_t n = similarityMatrix.size();
    vector<double> scores(n, 1.0);
    vector<double> tempScores(n, 0.0);
    vector<double> similaritySums(n, 0.0);

    for (size_t j = 0; j < n; ++j) {
        similaritySums[j] = std::accumulate(similarityMatrix[j].begin(), similarityMatrix[j].end(), 0.0);
    }

    for (int iter = 0; iter < maxIter; ++iter) {

        bool converged = true;

        for (size_t i = 0; i < n; ++i) {
            double new_score = (1 - d);
            for (size_t j = 0; j < n; ++j) {
                if (i != j && similaritySums[j] > 0) {
                    new_score += d * (similarityMatrix[j][i] / similaritySums[j]) * scores[j];
                }
            }
            tempScores[i] = new_score;

            if (std::fabs(new_score - scores[i]) > MIN_THRESHOLD) {
                converged = false;
            }
        }

        scores = tempScores;

        if (converged) {
            break;
        }
    }

    vector<int> ranks(n);
    std::iota(ranks.begin(), ranks.end(), 0);
    std::sort(ranks.begin(), ranks.end(), [&scores](int a, int b) {
        return scores[a] > scores[b];
    });

    return ranks;
}
