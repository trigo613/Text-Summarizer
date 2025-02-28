#include "TextRank.h"
#include "Constants.h"
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <atomic>

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


vector<vector<double>> TextRank::generateSimilarityMatrixMultiThreading(vector<SparseVector> &embeddings) {
    size_t N = embeddings.size();
    vector<vector<double>> similarityMatrix(N, vector<double>(N, 0.0));

    // Set diagonal values to 1.0 (self-similarity)
    for (size_t i = 0; i < N; i++) {
        similarityMatrix[i][i] = 1.0;
    }

    // Determine number of threads (use hardware concurrency or fallback to 1)
    unsigned int num_threads = std::thread::hardware_concurrency()/2;
    num_threads = (num_threads > 0) ? num_threads : 1;

    // Function to process a chunk of the matrix
    auto process_chunk = [&](size_t start_i, size_t end_i) {
        for (size_t i = start_i; i < end_i; ++i) {
            for (size_t j = i + 1; j < N; ++j) {
                double similarity = cosine_similarity(embeddings[i], embeddings[j]);
                similarityMatrix[i][j] = similarity;
                similarityMatrix[j][i] = similarity;
            }
        }
    };

    // Create and launch threads
    std::vector<std::thread> threads;
    size_t chunk_size = N / num_threads;

    for (unsigned int t = 0; t < num_threads; ++t) {
        size_t start_i = t * chunk_size;
        size_t end_i = (t == num_threads - 1) ? N : (t + 1) * chunk_size;
        threads.push_back(std::thread(process_chunk, start_i, end_i));
    }

    // Join all threads
    for (auto& thread : threads) {
        thread.join();
    }

    return similarityMatrix;
}


vector<int> TextRank::textRankMultiThreading(const vector<vector<double>> &similarityMatrix, int maxIter, double d) {
    size_t n = similarityMatrix.size();
    vector<double> scores(n, 1.0);
    vector<double> tempScores(n, 0.0);
    vector<double> similaritySums(n, 0.0);

    // Calculate similarity sums
    for (size_t j = 0; j < n; ++j) {
        similaritySums[j] = std::accumulate(similarityMatrix[j].begin(), similarityMatrix[j].end(), 0.0);
    }

    // Determine number of threads (use hardware concurrency or fallback to 1)
    unsigned int num_threads = std::thread::hardware_concurrency()/2;
    num_threads = (num_threads > 0) ? num_threads : 1;

    for (int iter = 0; iter < maxIter; ++iter) {
        std::atomic<bool> converged{true};

        // Function to process a chunk of nodes
        auto process_chunk = [&](size_t start_i, size_t end_i) {
            for (size_t i = start_i; i < end_i; ++i) {
                double new_score = (1 - d);
                for (size_t j = 0; j < n; ++j) {
                    if (i != j && similaritySums[j] > 0) {
                        new_score += d * (similarityMatrix[j][i] / similaritySums[j]) * scores[j];
                    }
                }
                tempScores[i] = new_score;

                if (std::fabs(new_score - scores[i]) > MIN_THRESHOLD) {
                    converged.store(false);
                }
            }
        };

        // Create and launch threads
        std::vector<std::thread> threads;
        size_t chunk_size = n / num_threads;

        for (unsigned int t = 0; t < num_threads; ++t) {
            size_t start_i = t * chunk_size;
            size_t end_i = (t == num_threads - 1) ? n : (t + 1) * chunk_size;
            threads.push_back(std::thread(process_chunk, start_i, end_i));
        }

        // Join all threads
        for (auto& thread : threads) {
            thread.join();
        }

        // Update scores
        scores = tempScores;

        if (converged.load()) {
            break;
        }
    }

    // Rank nodes based on final scores
    vector<int> ranks(n);
    std::iota(ranks.begin(), ranks.end(), 0);
    std::sort(ranks.begin(), ranks.end(), [&scores](int a, int b) {
        return scores[a] > scores[b];
    });

    return ranks;
}