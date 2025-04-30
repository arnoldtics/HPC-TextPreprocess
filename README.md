# HPC-TextPreprocess

## Description
Implementation of Natural Language preprocessing algorithms using High-Performance Computing (HPC) with MPI.

## Author and Contact
- Arnoldo Fernando Chue Sánchez: arnoldwork20@gmail.com

## License
GNU General Public License v3.0

## Affiliation
Final project for the High-Performance Computing (Supercomputing) 2025-2 class, designed and developed at the National Autonomous University of Mexico (UNAM), specifically at its National School of Higher Studies, Morelia Campus (ENES Morelia). This project is part of the Specialization Area of Information Sciences in the Bachelor's Program in Information Technologies for Science. The tests were performed in the Interdisciplinary Laboratory of Scientific Computing.

## Introduction
Natural Language Processing (NLP) is a field of Computer Science that has significantly grown in recent years and is expected to continue expanding in the near and medium-term future. However, a complete NLP project consists of multiple phases, with one of the most crucial being the first: cleaning the text dataset (corpus).

Corpus cleaning involves several key tasks, including:
- Tokenization: splitting each instance into individual words-tokens.
- Stop-words removal: eliminating common words from the language being processed.
- Lemmatization: converting words to their base form.

This project focuses on the tokenization and removal of stop-words using a parallelized approach with High-Performance Computing (HPC) and MPI.

## Justification
Although there are highly efficient techniques for tokenization and stop-word removal, processing large corpus remains a time-consuming task. In some cases, text preprocessing can take as long as training a neural network for NLP.

This represents a significant bottleneck which can be mitigated using High-Performance Computing. For example, in the author's project Thesis Cloud https://github.com/arnoldtics/Thesis_Cloud, nearly half a million thesis titles must be preprocessed before training the model. This process already requires a considerable amount of time. Therefore, in larger datasets (e.g., entire books), a parallelized algorithm would be extremely beneficial.

## Hypothesis
*A significant reduction in the time required for natural language preprocessing can be achieved by parallelizing the algorithms for tokenization and stop-words removal*

## General Objective
- Implement a parallelized natural language preprocessing algorithm using MPI, adaptable to any supercomputer architecture for processing large-scale corpora.

## Particular Objectives
1. Create an efficient representation for sets of stop-words in Spanish and English. 
2. Implement the natural language preprocessing algorithm.
3. Synchronize the parallel processes in the algorithm to achieve efficient parallelization.
4. Test the HPC algorithm on a supercomputer architecture 

## Toolset
The project was developed in C with MPI library for parallelization, and GNU Makefile. 

To provide a performance benchmark, the same algorithm was also implemented in Python for sequential execution.

## Usage Instructions & Requirements
To process a corpus, place its content in a .txt file
- **Python**. It only works for English corpus. To execute: copy the cleaner.py file (it is in Python_Sequential folder) to the same directory as your input text file. Run cleaner.py with two arguments: the input file name and the output file name
- **C Sequential**. It works with both (English - Spanish). Copy the C_Sequential folder. Edit the GNUmakefile to match your system's compiler configuration. Use the make command and then run the program with 3 arguments: input file name, output file name, and language (english or spanish). The input/output can be the relative path to the files.
- **C Parallelized**. It works with both (English - Spanish). Copy the C_Parallelized folder. Edit the GNUmakefile to match your system's compiler configuration, specially the route to the MPI library. Use the make command and then run the program with 3 arguments: input file name, output file name, and language (english or spanish). The input/output can be the relative path to the files. The number of threads can be defined during execution because the programm is totally adapted to the computer architecture.

## Algorithm Metodology
*This section explains how the algorithm works. Details about the parallelization process are convered in the next section.*

The algorith follows these steps:
1. It verifies the input/output files, as well as the language.
2. If validation is successful, the program reads each line from the input file and stores it in an array for processing. 
    - Each line has a default maximum length of 4096 characters. This limit can be increased by modifying line 12 in common.h. 
    - A line is represented as a struct containing the text and an index, which helps preserve the original order during parallel execution
3. Each line undergoes the following processing: 
    - Every character is checked to determine if it is alphanumeric. 
        - If it is a letter, it is converted to lowercase. 
        - In Spanish, additional checks are performed for special characters (accented letters, ñ and ü)
    - While processing characters, tokens (words) are identified using whitespaces as delimiters
    - Each token is checked against a list of stop words (using binary search). This list is sourced from DeepSeek.
    - Tokens that are not stop words are reconstructed into the cleaned version of the line.
4. Due to desynchronizacion during parallel processing, the lines are sorted back to the original order using their index.
5. The cleaned and ordered lines are written to the output file.

## Parallelization
The algorithm uses a master-worker model to achieve parallelism with MPI.
- The master thread (rank 0) is responsible for:
    1. Performing all initial validations (input/output files and language).
    2. Reading all lines from the input file and storing them in an array of custom line structs (each containing the text and its original index).
    3. Distributing lines dynamically to the worker threads for processing, along with a flag indicating the selected language.
    4. Applying the barber method for load balancing: whenever a worker finishes its task, the master immediately assigns it a new one.
    5. Once all lines have been processed, the master gathers the cleaned results, reorders them using the original indices, and writes them to the output file.
- The worker threads (rank > 0) follow this process:
    1. Send the cleaned line back to the master.
    2. Wait for a line and a language flag from the master.
    3. Apply the cleaning process described in the previous section (character filtering, tokenization, stop-word removal).

## Testing and Results
The algorithm was executed 10 times for each thread count on the CE computer of the Interdisciplinary Laboratory of Scientific Computing. The tests were conducted using different versions of the implementation: Python, C Sequential, and C Parallelized.

Plot description:
- The horizontal axis represents the number of threads used.
- The vertical axis indicates the execution time (in seconds).
- Each boxplot displays:
    - Mean
    - Median
    - Interquartile range (IQR)
- Colors distinguish the implementation type:
    - Purple for C Parallelized
    - Green for C Sequential
    - Blue for Python 

**UNAM Bachelor's degree theses titles:** 406,896 lines
![tesis](/img/boxplots/tesis.png)

**Catholic Bible:** 31,105 lines
![bible](/img/boxplots/biblia.png)
![bible](/img/boxplots/bibliaLog.png)

**Les Miserable Book by Victor Hugo:** 73,586 lines
![miserables](/img/boxplots/miserables.png)

## Conclusions

## References
- IBM. What are stemming and lemmatization: https://www.ibm.com/think/topics/stemming-lemmatization
- IBM. What is NLP (natural language processing): https://www.ibm.com/think/topics/natural-language-processing
- The Standford Natural Language Processing Group. Dropping common terms: stop words. https://nlp.stanford.edu/IR-book/html/htmledition/dropping-common-terms-stop-words-1.html