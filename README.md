# TF-IDF TextRank Summarizer

This C++ project implements a text summarization tool using TF-IDF (Term Frequency-Inverse StringVector Frequency) embeddings and the TextRank algorithm. It extracts the most important sentences from a given text file based on their significance within the document.
For more details, refer to the [original paper](https://web.eecs.umich.edu/~mihalcea/papers/mihalcea.emnlp04.pdf).


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

## Usage

Run the executable with the following command-line arguments:

```
./Text_Summarizer <input_file_path> <output_file_path> <num_sentences>
```

Example:
```
./Text_Summarizer Text.txt TextSummary.txt 5
```

- `<input_file_path>`: Path to the input text file
- `<output_file_path>`: Path to the output file where summarized sentences will be written
- `<num_sentences>`: Number of top sentences to include in the summary


## Summary of Article: The Benefits of Coffee
The following sentences are a short summary, generated by our algorithm, highlighting the top five most important sentences from the article ["9 Reasons Why the Right Amount of Coffee Is Good for You".](https://www.hopkinsmedicine.org/health/wellness-and-prevention/9-reasons-why-the-right-amount-of-coffee-is-good-for-you) You can find the complete summary in TextSummary.txt, and the full article in Text.txt.


> - Recent studies found that coffee drinkers are less likely to die from some of the leading causes
> of death in women: coronary heart disease, stroke, diabetes and kidney disease
>
> - In fact, researchers found that women age 65 and older who drank two to three cups of coffee a day were
> less likely to develop dementia in general
>
> - Drinking one to two cups of coffee a day may help ward off heart failure, when a weakened heart
> has difficulty pumping enough blood to the body
>
> - Study after study indicates you could be getting more from your favorite morning beverage
>than you thought: Coffee is chock full of substances that may help guard against conditions more common in women,
>including Alzheimer’s disease and heart disease
>
> - You are less likely to develop Parkinson's disease

## Top 3 sentences extracted from the first book of Harry Potter.
The following sentences are extracted from the book Harry Potter and the Sorcerer's Stone. These 3 sentences scored the highest using the TextRank algorithm.


> - Don’t you understand? If Snape gets hold of the Stone, Voldemort’s coming back! Haven’t you heard what it was like when he was trying to take over? There won’t be any Hogwarts to get expelled from! He’ll  
> flatten it, or turn it into a school for the Dark Arts! Losing points doesn’t matter anymore, can’t you see? D’you think he’ll leave you and your families alone if Gryffindor wins the House Cup? If I get caught
> before I can get to the Stone, well, I’ll have to go back to the Dursleys and wait for Voldemort to find me there, it’s only dying a bit later than I would have, because I’m never going over to the Dark Side!  
> I’m going through that trapdoor tonight and nothing you two say is going to stop me! Voldemort killed my parents, remember?” He glared at them.
>
> - But Harry only had one thought in his head, which was to get back in front of the mirror, and Ron wasn’t going to stop him.
>
> - What if he wasn’t chosen at all? What if he just sat there with the hat over his eyes for ages, until Professor McGonagall jerked it off his head and said there had obviously been a mistake and he’d better
> get back on the train? When Neville Longbottom, the boy who kept losing his toad, was called, he fell over on his way to the stool.

## Acknowledgments

- Original TextRank paper: "TextRank: Bringing Order into Texts" by Rada Mihalcea and Paul Tarau
