# TF-IDF TextRank Summarizer

This C++ project implements a text summarization tool using TF-IDF (Term Frequency-Inverse Document Frequency) embeddings and the TextRank algorithm. It extracts the most important sentences from a given text file based on their significance within the document.

## Process

- Reads input text from a file
- Generates TF-IDF embeddings for each sentence
- Applies the TextRank algorithm to rank sentences
- Outputs the top N most important sentences to a file

## Building the Project

1. Clone the repository:
   ```
   git clone https://github.com/trigo613/Text-Summarizer.git
   cd Text-Summarizer
   ```

2. Create a build directory and run CMake:
   ```
   mkdir build
   cd build
   cmake ..
   make
   ```

## Usage

Run the executable with the following command-line arguments:

```
./TextSummarization <input_file_path> <output_file_path> <num_sentences>
```

- `<input_file_path>`: Path to the input text file
- `<output_file_path>`: Path to the output file where summarized sentences will be written
- `<num_sentences>`: Number of top sentences to include in the summary

Example:
```
./TextSummarization input.txt output.txt 10
```

## Example

These sentences are the contents of TextSummary.txt, which is a summary written by our algorithm of the original TextRank paper found in Text.txt:

1. The TextRank Model
2. Graph-based ranking algorithms are essentially a way of deciding the importance of a vertex within a graph, based on global information recursively drawn from the entire graph
3. In short, a graph-based ranking algorithm is a way of deciding on the importance of a vertex within a graph, by taking into account global information recursively computed from the entire graph, rather than relying only on local vertex-specific information
4. Hence, the score associated with a vertex is determined based on the votes that are cast for it, and the score of the vertices casting these votes
5. The higher the number of votes that are cast for a vertex, the higher the importance of the vertex
6. Moreover, the importance of the vertex casting the vote determines how important the vote itself is, and this information is also taken into account by the ranking model

## License

This project is open-source. Please refer to the LICENSE file in the repository for more details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Acknowledgments

- Original TextRank paper: "TextRank: Bringing Order into Texts" by Rada Mihalcea and Paul Tarau
- All contributors to this project

For more information or to report issues, please visit the [project repository](https://github.com/trigo613/Text-Summarizer).
