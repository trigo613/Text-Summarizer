## Process 

### 1) Read the input text file  
### 2) Separate the text into sentences by a pre-defined delimeter ('.' in this case).
Part of this step includes also removing dots from words that frequently have them so that we don't separate the sentences at the wrong place.  
For example : "**Mr.** Bob works in **construction.**" &rightarrow; "Mr Bob works in construction."
### 3) Remove Non-Standard characters like $%@, lower capital letters and tokenize digit sequences.
For example "**Bob** has **$500**" &rightarrow; "bob has **500**" &rightarrow; "bob has 3_DIGIT_TOKEN".  
For an LLM removing this information would not be beneficial but when using TfIdf in a small set-up like here this might increase performance.
### 4) Create Vocabulary set.
Here we create a set of all the words in our file.  
For example: "A dog and a cat are staring at a cat." &rightarrow; {a, dog, and, cat, are ,staring, at}.

### 5) The Sparse Vector Class

The SparseVector class efficiently represents high-dimensional vectors where most elements are zero, leveraging an unordered map to store only non-zero values. This approach significantly reduces memory usage compared to storing the entire vector.

Formally, given a vector $v = [v_1, v_2, \dots, v_n]$, where $v_i$ represents the $i$-th element, the SparseVector stores a mapping of indices to their corresponding non-zero values.

For example, consider the vector:

$$
v = [0, 1, 0, 0, 0, 2, 0, \dots, 0, 1, 0]
$$

This vector would be represented by the SparseVector as:

$$
\{(1, 1), (5, 2), (n-1, 1)\}
$$

Where each pair $(i, v_i)$ indicates that the $i$-th element of the vector has a value of $v_i$.  
This is done to reduce memory usage. (In code, this is represented using an unordered map.)

### 6) Generate Word Count Vectors for Each Sentence.

Each sentence is transformed into a word count vector, representing the frequency of each word within the sentence.

Let $V = \{w_1, w_2, \dots, w_n\}$ represent the vocabulary, where $w_i$ is the $i$-th word. For a given sentence $S$, the word count vector $v_S = [v_1, v_2, \dots, v_n]$ is constructed such that:

* $v_i$ represents the number of times word $w_i$ appears in sentence $S$.
* If word $w_i$ does not appear in $S$, then $v_i = 0$.

For example, given the sentence "I eat an apple," the resulting word count vector might look like:

$$
v_S = [0, 0, \dots, 1, 1, 0, 0, \dots, 1, 0, 1, 0]
$$

The SparseVector class efficiently stores this information by only recording the non-zero indices and their associated counts.

### 7) Generate TfIdf Vectors for Each Sentence

**Term Frequency-Inverse Document Frequency (TfIdf)** adjusts the word count vectors by emphasizing words that are frequent in a specific sentence but rare across the entire corpus.

Given a word count vector

$$
\mathbf{v} = [v_1, v_2, \dots, v_n]
$$

the **TfIdf** transformation updates each word's frequency \( v_i \) using the formula:

$$
\text{TfIdf}(v_i) = v_i \times \log\left(\frac{N}{df_i}\right)
$$

Where:  
* **`v_i`** is the raw term frequency of word \( i \) in the sentence.  
* **`N`** is the total number of documents (or sentences).  
* **`df_i`** is the document frequency of word \( i \), i.e., the number of documents containing word \( i \).

This transformation gives higher weight to terms that are unique to specific sentences, while reducing the importance of common terms across the corpus.

### 8) Generate Similarity Matrix (Weighted Fully Connected Graph)

We define a similarity matrix of size N×N, where N is the number of sentences. The value at index i, j represents the cosine similarity between sentence i and sentence j. The formula for cosine similarity is:

$$
\text{cosine similarity}(v_\text{i}, v_\text{j}) = \frac{v_\text{i} \cdot v_\text{j}}{\|v_\text{i}\| \|v_\text{j}\|}
$$


Since the similarity matrix is symmetric, i.e., M[i][j] = M[j][i], we only need to calculate the values for one half of the matrix, reducing the number of calculations by half.  
**This process is highly parallelizable and is implemented using multi-threading.**

### 9) Apply the TextRank algorithm

The TextRank algorithm, iteratively updates a score for each sentence based on the similarity connections within the fully connected graph. The main calculation in this process is:

$$
S(V_i) = (1 - d) + d \sum_{j \neq i} \frac{\text{similarity}(V_j, V_i)}{\text{outDegree}(V_j)} S(V_j)
$$

Where:

* **`S(V_i)`** represents the score of sentence (node) `V_i`.
* **`d`** is the damping factor (check paper for full explanation).
* **`similarity(V_j, V_i)`** is the similarity score between sentences `V_j` and `V_i` (as calculated in the previous step).
* **`outDegree(V_j)`** is defined as:



$$
\text{outDegree}(V_j) = \sum_{k \neq j} \text{similarity}(V_j, V_k)
$$


* **`S(V_j)`** is the current score of sentence `V_j`.

The algorithm iterates this process until the scores converge (i.e., the changes in scores become very small) or a maximum number of iterations is reached. Finally, the sentences are ranked based on their converged scores.   
**This process is highly parallelizable and is implemented using multi-threading.**
<img src="https://github.com/user-attachments/assets/a5c4bb13-a492-436e-960c-afb3133b95d9" width="800"/>

### 10) Write to the output file the N sentences with the highest score
