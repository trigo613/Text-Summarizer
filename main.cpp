#include <iostream>
#include <algorithm>
#include <numeric>
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
#define NUM_ARGUMENTS 4

std::string readFromFile(const std::string &filePath) {
    std::ifstream file(filePath);
    std::string text;
    std::string line;
    while (std::getline(file, line)) {
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

int validateArgs(int argc, char *argv[]) {
    if (argc != NUM_ARGUMENTS) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> <number_of_sentences>" << std::endl;
        return 1;
    }
    try {
        if (std::stoi(argv[NUM_SENTENCES_INDEX]) <= 0) {
            throw std::invalid_argument("Number of sentences must be positive.");
        }
    } catch (const std::invalid_argument &) {
        std::cerr << "Error: The number of sentences must be a valid integer." << std::endl;
        return 1;
    }
    return 0;
}


int main(int argc, char *argv[]) {

    if(validateArgs(argc, argv) != 0) {
        return 1;
    }

    std::string inputFilePath = argv[INPUT_FILE_INDEX];
    std::string outputFilePath = argv[OUTPUT_FILE_INDEX];
    int numSentences = std::stoi(argv[NUM_SENTENCES_INDEX]);

    std::string text = readFromFile(inputFilePath);

    text = DocumentParser::removeWordsWithPeriods(text);

    StringVector sentences = DocumentParser::splitToSentences(text, false);

    SentenceList clean_sentence_list = DocumentParser::splitSentencesToWords(sentences, true);

    std::cout << "Number of sentences: " << clean_sentence_list.size() << std::endl;

    TfIdf tfIdf = TfIdf();

    tfIdf.fit(clean_sentence_list);

    SentenceEmbeddingList embeddings = tfIdf.transform(clean_sentence_list);

    SimilarityMatrix similarityMatrix = TextRank::generateSimilarityMatrix(embeddings);

    // Perform TextRank and get ranks
    std::vector<int> ranks = TextRank::textRank(similarityMatrix, 100, 0.85);

    // Output the top ranked sentences
    for (int i = 0; i < numSentences && i < ranks.size(); i++) {
        std::cout << "Rank: " << i + 1 << ", Sentence: " << sentences[ranks[i]] << std::endl;
    }

    // Prepare summary to write to output file
    std::string summary;
    for (size_t i = 0; i < numSentences && i < ranks.size(); i++) {
        summary += sentences[ranks[i]] + "\n\n";
    }

    // Write summary to output file
    writeToFile(outputFilePath, summary);
    return 0;
}
