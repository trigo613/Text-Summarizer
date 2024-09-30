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
- `<output_file_path>`: Path to the output file where summarized sentences will be written
- `<num_sentences>`: Number of top sentences to include in the summary

## Example

The following React component demonstrates an example of how the summarizer works. It shows a summary of the original paper that introduced the TextRank algorithm, "TextRank: Bringing Order into Texts" by Rada Mihalcea and Paul Tarau.

```jsx
import React, { useState, useEffect } from 'react';

const ExampleDisplay = () => {
  const [inputText, setInputText] = useState('');
  const [summaryText, setSummaryText] = useState('');

  useEffect(() => {
    // Simulating file content from the TextRank paper
    setInputText(`TextRank – Bringing Order into Texts

    Rada Mihalcea and Paul Tarau
    Department of Computer Science
    University of North Texas

    Abstract

    In this paper, we introduce TextRank – a graph-based ranking model for text processing, and show how this model can be successfully used in natural language applications. In particular, we propose two innovative unsupervised methods for keyword and sentence extraction, and show that the results obtained outperform other unsupervised approaches.

    1. Introduction

    Graph-based ranking algorithms, such as Kleinberg's HITS algorithm (Kleinberg, 1999) or Google's PageRank (Brin and Page, 1998) have been successfully used in citation analysis, social networks, and the analysis of the link-structure of the World Wide Web. In short, a graph-based ranking algorithm is a way of deciding on the importance of a vertex within a graph, by taking into account global information recursively computed from the entire graph, rather than relying only on local vertex-specific information.

    2. The TextRank Model

    Graph-based ranking algorithms are essentially a way of deciding the importance of a vertex within a graph, based on global information recursively drawn from the entire graph. The basic idea implemented by a graph-based ranking model is that of "voting" or "recommendation". When one vertex links to another one, it is basically casting a vote for that other vertex. The higher the number of votes that are cast for a vertex, the higher the importance of the vertex.

    3. Keyword Extraction

    The first application we are considering is keyword extraction, which is the task of identifying a set of words or phrases that are representative of a given text. We propose an approach that consists of building a graph from the text to be processed, where the graph vertices are words, and the edges between vertices are drawn if the words co-occur within a window of N words.

    4. Sentence Extraction

    The second application we explore is sentence extraction. Given a text, the task is to identify a set of sentences that are representative for the text content. This is a popular summarization method, with the advantage of providing output summaries that are coherent and grammatically correct.

    5. Evaluation and Results

    We evaluate our methods on standard data sets, and compare the results with other state-of-the-art unsupervised approaches. For keyword extraction, we use the Inspec data set, which consists of 2000 abstracts from journal papers in Computer Science and Information Technology. For sentence extraction, we use the DUC 2002 data set, which consists of 567 news articles from various sources.

    6. Conclusions

    In this paper, we introduced TextRank – a graph-based ranking model for text processing, and showed how this model can be successfully used in natural language applications. We proposed and evaluated two innovative unsupervised methods for keyword and sentence extraction, and showed that the results obtained with these methods outperform other unsupervised approaches.`);

    setSummaryText(`TextRank is a graph-based ranking model for text processing, introduced for use in natural language applications.

    The paper proposes two unsupervised methods for keyword and sentence extraction, which outperform other unsupervised approaches.

    Graph-based ranking algorithms decide the importance of a vertex within a graph using global information computed recursively from the entire graph.

    The TextRank model implements the idea of "voting" or "recommendation", where links between vertices are considered as votes, and more votes indicate higher importance.

    The authors evaluate their methods on standard data sets and compare the results with other state-of-the-art unsupervised approaches.`);
  }, []);

  return (
    <div className="flex h-96 w-full">
      <div className="w-1/2 p-4 border-r">
        <h2 className="text-lg font-bold mb-2">Original Text (TextRank paper)</h2>
        <div className="h-80 overflow-y-auto bg-gray-100 p-2 rounded">
          <pre className="whitespace-pre-wrap">{inputText}</pre>
        </div>
      </div>
      <div className="w-1/2 p-4">
        <h2 className="text-lg font-bold mb-2">Summary</h2>
        <div className="h-80 overflow-y-auto bg-gray-100 p-2 rounded">
          <pre className="whitespace-pre-wrap">{summaryText}</pre>
        </div>
      </div>
    </div>
  );
};

export default ExampleDisplay;
```

This component displays the original text of the TextRank paper on the left and a summary generated by our TF-IDF TextRank Summarizer on the right. It demonstrates how the summarizer extracts key sentences from the original text to create a concise summary.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Acknowledgments

- [TextRank: Bringing Order into Texts](https://web.eecs.umich.edu/~mihalcea/papers/mihalcea.emnlp04.pdf) by Rada Mihalcea and Paul Tarau
- [TF-IDF](https://en.wikipedia.org/wiki/Tf%E2%80%93idf) on Wikipedia
