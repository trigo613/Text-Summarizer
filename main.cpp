#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <stdexcept>
#include <cmath>
#include "TfIdf.h"
#include "DocumentParser.h"
#include "Constants.h"


#define MIN_THRESHOLD 0.0000001

double cosine_similarity(const std::vector<double> &A, const std::vector<double> &B) {
    double dot_product = 0.0, normA = 0.0, normB = 0.0;
    for (size_t i = 0; i < A.size(); ++i) {
        dot_product += A[i] * B[i];
        normA += A[i] * A[i];
        normB += B[i] * B[i];
    }
    return dot_product / (sqrt(normA) * sqrt(normB));
}


SimilarityMatrix generateSimilarityMatrix(const SentenceEmbeddingList &embeddings) {
    size_t N = embeddings.size();
    SimilarityMatrix similarityMatrix(N, std::vector<double>(N, 0.0));
    for (size_t i = 0; i < N; i++) {
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



std::vector<int> textRank(const SimilarityMatrix& similarityMatrix, int maxIter, double d) {
    size_t n = similarityMatrix.size();
    std::vector<double> scores(n, 1.0);
    std::vector<double> tempScores(n, 0.0);
    for (int iter = 0; iter < maxIter; ++iter) {
        bool converged = true;
        for (size_t i = 0; i < n; ++i) {
            tempScores[i] = (1 - d);
            for (size_t j = 0; j < n; ++j) {
                if (i != j) {
                    double similaritySum = std::accumulate(similarityMatrix[j].begin(),
                                                           similarityMatrix[j].end(),
                                                           0.0);
                    if (similaritySum > 0) {
                        tempScores[i] += d * (similarityMatrix[j][i] / similaritySum) * scores[j];
                    }
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


std::string readFromFile(const std::string &filePath) {
    std::ifstream file(filePath);
    std::string text;
    std::string line;
    int k = 0;
    while (std::getline(file, line) && k++ < 2000) {
        text += line + " ";
    }
    file.close();
    return text;
}

std::string writeToFile(const std::string &filePath, const std::string &text) {
    std::ofstream file(filePath);
    file << text;
    file.close();
    return filePath;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> <number_of_sentences>" << std::endl;
        return 1;
    }

    std::string inputFilePath = argv[1];
    std::string outputFilePath = argv[2];
    int numSentences;
    try {
        numSentences = std::stoi(argv[3]);
        if (numSentences <= 0) {
            throw std::invalid_argument("Number of sentences must be positive.");
        }
    } catch (const std::invalid_argument&) {
        std::cerr << "Error: The number of sentences must be a valid integer." << std::endl;
        return 1;
    }

    std::string text = readFromFile(inputFilePath);
    text = DocumentParser::removePeriodFromWords(text);
    text = DocumentParser::removeWordsWithDots(text);

    // Split text into sentences
    SentenceList sentences = DocumentParser::splitDocumentToSentences(text);
    std::cout << "Number of sentences: " << sentences.size() << std::endl;

    // Generate embeddings and similarity matrix
    SentenceEmbeddingList embeddings = TfIdf::generateEmbeddings(sentences);
    SimilarityMatrix similarityMatrix = generateSimilarityMatrix(embeddings);

    // Split document into sentences for ranking
    Document doc = DocumentParser::splitToSentences(text);
    //DocumentParser::cleanDocument(doc);

    // Perform TextRank and get ranks
    std::vector<int> ranks = textRank(similarityMatrix, numSentences, 0.85);

    // Output the top ranked sentences
    for (int i = 0; i < numSentences && i < ranks.size(); i++) {
        std::cout << "Rank: " << i + 1 << ", Sentence: " << doc[ranks[i]] << std::endl;
    }

    // Prepare summary to write to output file
    std::string summary;
    for (size_t i = 0; i < numSentences && i < ranks.size(); i++) {
        summary += doc[ranks[i]] + "\n\n";
    }

    // Write summary to output file
    writeToFile(outputFilePath, summary);
    return 0;
}
