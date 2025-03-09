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
- Tokenization: splitting each instance into individual words.
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
The project will be developed using C with MPI library and GNU Makefile.

## Usage Instructions & Requirements

## General System Architecture

## Metodology

## Testing and Results

## Conclusions

## References
- IBM. What are stemming and lemmatization: https://www.ibm.com/think/topics/stemming-lemmatization
- IBM. What is NLP (natural language processing): https://www.ibm.com/think/topics/natural-language-processing
- The Standford Natural Language Processing Group. Dropping common terms: stop words. https://nlp.stanford.edu/IR-book/html/htmledition/dropping-common-terms-stop-words-1.html