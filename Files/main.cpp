#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>
#include "TfIdf.h"
#include "DocumentParser.h"
#include "Constants.h"
#include "TextRank.h"

#define INPUT_FILE_INDEX 1
#define OUTPUT_FILE_INDEX 2
#define NUM_SENTENCES_INDEX 3
#define NUM_THREADS_INDEX 4
#define MIN_NUM_ARGUMENTS 4
#define MAX_NUM_ARGUMENTS 5

string readFromFile(const string &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        exit(1);
    }
    string text;
    string line;
    while (std::getline(file, line)) {
        text += line + " ";
    }
    file.close();
    return text;
}

string writeToFile(const string &filePath, const string &text) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        exit(1);
    }
    file << text;
    file.close();
    return filePath;
}


int main(int argc, char *argv[]) {

    if (argc < MIN_NUM_ARGUMENTS || argc > MAX_NUM_ARGUMENTS) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> <number_of_sentences> [num_threads]" << std::endl;
        return 1;
    }

    std::string inputFilePath = argv[INPUT_FILE_INDEX];
    std::string outputFilePath = argv[OUTPUT_FILE_INDEX];
    int numSentences;
    int numThreads = 1;

    try {
        numSentences = std::stoi(argv[NUM_SENTENCES_INDEX]);
        if (numSentences <= 0) {
            throw std::invalid_argument("Number of sentences must be positive.");
        }
    } catch (const std::invalid_argument &) {
        std::cerr << "Error: The number of sentences must be a valid positive integer." << std::endl;
        return 1;
    }
    try {
        if (argc == MAX_NUM_ARGUMENTS) {
            numThreads = std::stoi(argv[NUM_THREADS_INDEX]);
            if (numThreads <= 0) {
                throw std::invalid_argument("Number of threads must be positive.");
            }
        }
    } catch (const std::invalid_argument &) {
        std::cerr << "Error: The number of threads must be a valid positive integer or -1 for hardware concurrency." << std::endl;
        return 1;
    }

    // Read input file

    std::cout << "Reading input file..." << std::endl;

    string text = readFromFile(inputFilePath);

    DocumentParser::removeWordsWithPeriodsInplace(text);

    // Split text into sentences

    vector<string> sentences = DocumentParser::splitToSentences(text);

    std::cout << "Number of sentences: " << sentences.size() << std::endl;

    std::cout << "Tokenizing sentences..." << std::endl;

    // Tokenize sentences

    vector<vector<string>> tokenized_sentences;

    tokenized_sentences.reserve(sentences.size());

    for (const auto &sentence: sentences) {
        string s = sentence;
        vector<string> words = DocumentParser::splitStringByDelim(s,' ');
        DocumentParser::tokenizeSentenceInplace(words);
        tokenized_sentences.push_back(words);
    }

    std::cout << "Generating embeddings..." << std::endl;

    // Fit the TfIdf model

    TfIdf tfIdf = TfIdf();

    tfIdf.fit(tokenized_sentences);

    vector<SparseVector> embeddings = tfIdf.transform(tokenized_sentences);

    std::cout << "Generating similarity matrix..." << std::endl;

    // Generate similarity matrix

    //vector<vector<double>> similarityMatrix = TextRank::generateSimilarityMatrix(embeddings);
    vector<vector<double>> similarityMatrix = TextRank::generateSimilarityMatrixMultiThreading(embeddings, numThreads);

    std::cout << "Running TextRank algorithm..." << std::endl;

    // Perform TextRank and get ranks

    //vector<int> ranks = TextRank::textRank(similarityMatrix, 100, 0.85);
    vector<int> ranks = TextRank::textRankMultiThreading(similarityMatrix, 100, 0.85, numThreads);

    // Output the top ranked sentences
    for (int i = 0; i < numSentences && i < ranks.size(); i++) {
        std::cout << "Rank: " << i + 1 << ", Sentence: " << sentences[ranks[i]] << std::endl;
    }

    // Prepare summary to write to output file
    string summary;
    for (size_t i = 0; i < numSentences && i < ranks.size(); i++) {
        summary += sentences[ranks[i]] + "\n\n";
    }

    // Write summary to output file
    writeToFile(outputFilePath, summary);
    return 0;
}
