# TF-IDF TextRank Summarizer

This C++ project implements a text summarization tool using TF-IDF (Term Frequency-Inverse Document Frequency) embeddings and the TextRank algorithm. It extracts the most important sentences from a given text file based on their significance within the document.

## Features

- Reads input text from a file
- Generates TF-IDF embeddings for each sentence
- Applies the TextRank algorithm to rank sentences
- Outputs the top N most important sentences to a file

## Prerequisites

- C++17 or later
- CMake 3.10 or later
- Boost library (for string tokenization and filesystem operations)

## Building the Project

1. Clone the repository:
   ```
   git clone https://github.com/yourusername/tfidf-textrank-summarizer.git
   cd tfidf-textrank-summarizer
   ```

2. Create a build directory and run CMake:
   ```
   mkdir build
   cd build
   cmake ..
   ```

3. Build the project:
   ```
   make
   ```

## Usage

Run the executable with the following command-line arguments:

```
./tfidf_textrank_summarizer <input_file_path> <output_file_path> <num_sentences>
```

- `<input_file_path>`: Path to the input text file
- `output_file_path>`: Path to the output file where summarized sentences will be written
- `<num_sentences>`: Number of top sentences to include in the summary

## Example

```
./tfidf_textrank_summarizer ../data/input.txt ../data/summary.txt 5
```

This command will read the text from `../data/input.txt`, process it, and write the 5 most important sentences to `../data/summary.txt`.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Acknowledgments

- [TextRank: Bringing Order into Texts](https://web.eecs.umich.edu/~mihalcea/papers/mihalcea.emnlp04.pdf) by Rada Mihalcea and Paul Tarau
- [TF-IDF](https://en.wikipedia.org/wiki/Tf%E2%80%93idf) on Wikipedia
