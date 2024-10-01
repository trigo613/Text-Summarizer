//
// Created by skche on 01/10/2024.
//

#include <thread>
#include "TextRank.h"
#include "Constants.h"


double TextRank::cosine_similarity(SparseVector a, SparseVector b) {
    return dot(a, b) / (a.norm() * b.norm());
}


SimilarityMatrix TextRank::generateSimilarityMatrix(const SentenceEmbeddingList &embeddings) {
    size_t N = embeddings.size();
    SimilarityMatrix similarityMatrix(N, std::vector<double>(N, 0.0));

    // Function to compute similarity for a range of indices
    auto compute_similarity = [&](size_t start, size_t end) {
        for (size_t i = start; i < end; ++i) {
            if (i % 100 == 0) {
                std::cout << "Calculating similarity for sentence " << i << " of " << N << std::endl;
            }

            for (size_t j = i + 1; j < N; ++j) {
                double similarity = cosine_similarity(embeddings[i], embeddings[j]);
                similarityMatrix[i][j] = similarity;
                similarityMatrix[j][i] = similarity;
            }
        }
    };

    // Determine the number of threads to use (you can adjust this number)
    const size_t num_threads = std::thread::hardware_concurrency(); // Number of available threads
    std::vector<std::thread> threads;
    size_t chunk_size = N / num_threads;

    // Create threads to handle different parts of the matrix
    for (size_t t = 0; t < num_threads; ++t) {
        size_t start = t * chunk_size;
        size_t end = (t == num_threads - 1) ? N : start + chunk_size;
        threads.emplace_back(compute_similarity, start, end);
    }

    // Join all the threads (wait for them to finish)
    for (auto& thread : threads) {
        thread.join();
    }

    // Set diagonal elements to 1.0
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

    // Calculate similarity sums in parallel
    const size_t num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> sum_threads;
    size_t chunk_size = n / num_threads;

    auto calculate_similarity_sums = [&](size_t start, size_t end) {
        for (size_t j = start; j < end; ++j) {
            similaritySums[j] = std::accumulate(similarityMatrix[j].begin(),
                                                similarityMatrix[j].end(), 0.0);
        }
    };

    for (size_t t = 0; t < num_threads; ++t) {
        size_t start = t * chunk_size;
        size_t end = (t == num_threads - 1) ? n : start + chunk_size;
        sum_threads.emplace_back(calculate_similarity_sums, start, end);
    }

    for (auto& thread : sum_threads) {
        thread.join();
    }

    // Main iteration loop
    for (int iter = 0; iter < maxIter; ++iter) {
        //print iteration number
        std::cout << "Iteration: " << iter << std::endl;
        bool converged = true;
        std::vector<bool> local_converged(num_threads, true);
        std::vector<std::thread> threads;

        auto update_scores = [&](size_t start, size_t end, size_t thread_id) {
            for (size_t i = start; i < end; ++i) {
                double new_score = (1 - d);
                for (size_t j = 0; j < n; ++j) {
                    if (i != j && similaritySums[j] > 0) {
                        new_score += d * (similarityMatrix[j][i] / similaritySums[j]) * scores[j];
                    }
                }
                tempScores[i] = new_score;

                // Check for convergence
                if (std::fabs(new_score - scores[i]) > MIN_THRESHOLD) {
                    local_converged[thread_id] = false;
                }
            }
        };

        // Create threads for score updates
        for (size_t t = 0; t < num_threads; ++t) {
            size_t start = t * chunk_size;
            size_t end = (t == num_threads - 1) ? n : start + chunk_size;
            threads.emplace_back(update_scores, start, end, t);
        }

        // Join all threads
        for (auto& thread : threads) {
            thread.join();
        }

        // Check if any thread reported non-convergence
        converged = std::all_of(local_converged.begin(), local_converged.end(),
                                [](bool v) { return v; });

        scores = tempScores;

        if (converged) {
            break;
        }
    }

    // Create ranking
    std::vector<int> ranks(n);
    std::iota(ranks.begin(), ranks.end(), 0);
    std::sort(ranks.begin(), ranks.end(), [&scores](int a, int b) {
        return scores[a] > scores[b];
    });

    return ranks;
}