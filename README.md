# TF-IDF TextRank Summarizer

This C++ project implements a text summarization tool using TF-IDF (Term Frequency-Inverse Document Frequency) embeddings and the TextRank algorithm. It extracts the most important sentences from a given text file based on their significance within the document.
For more details, refer to [Process.md](./Process.md) or the [original paper](https://web.eecs.umich.edu/~mihalcea/papers/mihalcea.emnlp04.pdf).
## Build Instructions  

### Prerequisites  
Ensure you have the following installed:  
- A C++ compiler supporting C++17 or later (`g++`, `clang++`, or MSVC)  
- `CMake` (version 3.24 or later)  

### Cloning the Repository  
First, clone the repository and navigate to the project directory:  
```bash
git clone https://github.com/trigo613/Text-Summarizer.git
cd Text-Summarizer/Files
```

### Building with CMake  
To build the project using `CMake`, follow these steps:  

```bash
mkdir build
cd build
cmake .
cmake --build .    
```
This will generate the `Text_Summarizer` executable.  
## Usage

Run the executable with the following command-line arguments:

```
./Text_Summarizer <input_file_path> <output_file_path> <num_sentences> [num_threads]
```

- `<input_file_path>`: Path to the input text file
- `<output_file_path>`: Path to the output file where the summarized document will be written
- `<num_sentences>`: Number of top sentences to include in the summary
- `[num_threads]` : Number of threads to use, 1 by default. (-1 to use hardware_concurrency)
  
Example:
```
./Text_Summarizer Text.txt TextSummary.txt 5
```

## Links  
- **[Results.md](./Results.md)** – See an example output of the algorithm after summarization.  
- **[Process.md](./Process.md)** – Learn about the steps involved in the summarization process and the mathematical foundation behind it.  


## Acknowledgments

- Original TextRank paper: "TextRank: Bringing Order into Texts" by Rada Mihalcea and Paul Tarau
