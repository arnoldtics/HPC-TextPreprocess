# HPC-TextPreprocess

## Description
Implementation of Natural Language preprocessing algorithms using High-Performance Computing (HPC) with MPI.

## Author and Contact
- Arnoldo Fernando Chue Sánchez: arnoldwork20@gmail.com

## License
GNU General Public License v3.0

## Affiliation
Final project for the High-Performance Computing (Supercomputing) 2025-2 class, designed and developed at the National Autonomous University of Mexico (UNAM), specifically at its National School of Higher Studies, Morelia Campus (ENES Morelia). This project is part of the Specialization Area of Information Sciences in the Bachelor's Program in Information Technologies for Science.

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

## General System Architecture

## Metodology

## Testing and Results

## Conclusions

## References
- IBM. What are stemming and lemmatization: https://www.ibm.com/think/topics/stemming-lemmatization
- IBM. What is NLP (natural language processing): https://www.ibm.com/think/topics/natural-language-processing
- The Standford Natural Language Processing Group. Dropping common terms: stop words. https://nlp.stanford.edu/IR-book/html/htmledition/dropping-common-terms-stop-words-1.html