#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// Stack security
#define MAX_STATIC_LINES 500000  

// Maximum number of letters in a line
#define MAX_SIZE 4096

// Avoiding declare the stopwords and their size twice
extern const char *SPANISH_STOP_WORDS[];
extern const size_t NUMBER_OF_SPANISH_STOP_WORDS;

// Structure to store a line and its index (original position) in the file
typedef struct{
    int index;
    char *line;
} Line;

typedef struct {
    int index;
    char* original_line;  // Original line (master)
    char* cleaned_line;   // Cleaned line (worker)
    // int processed;        // Flag to indicate if it was processed
} MPILine;

// Funtion to sort our Line structure
int comp_line_struct(const void *a, const void *b);

// Function to check if a word is a stop word
// It isn't case sensitive
int is_stop_word(const char *word);

// Function to remove stop words from a line
char* remove_stop_words(const char* line);

// Function to remove punctuation from a line (it keeps numbers)
void remove_punctuation(char *str);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* COMMON_H */