// Dealing with windows compatibility
#ifdef _WIN32
#include <windows.h>
#define strcasecmp _stricmp
#else
#include <strings.h>  
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "common.h"

// Spanish stop words (sorted and ready for binary search)
// Provided by deepseek
const char *SPANISH_STOP_WORDS[] = {
    "a", "al", "algo", "algún", "alguna", "algunas", "alguno", "algunos", "ante", 
    "antes", "como", "con", "contra", "cual", "cuando", "de", "del", "desde", 
    "donde", "durante", "e", "el", "ella", "ellas", "ellos", "en", "entre", "era", 
    "erais", "eran", "eras", "eres", "es", "esa", "esas", "ese", "eso", "esos", 
    "esta", "estaba", "estabais", "estaban", "estabas", "estad", "estada", "estadas", 
    "estado", "estados", "estamos", "estando", "estar", "estaremos", "estará", "estarán", 
    "estarás", "estaré", "estaréis", "estaría", "estaríais", "estaríamos", "estarían", 
    "estarías", "estas", "este", "esto", "estos", "estoy", "estuve", "estuviera", 
    "estuvierais", "estuvieran", "estuvieras", "estuvieron", "estuviese", "estuvieseis", 
    "estuviesen", "estuvieses", "estuvimos", "estuviste", "estuvisteis", "estuvo", 
    "fue", "fuera", "fuerais", "fueran", "fueras", "fueron", "fuese", "fueseis", 
    "fuesen", "fueses", "fui", "fuimos", "fuiste", "fuisteis", "ha", "habida", 
    "habidas", "habido", "habidos", "habiendo", "habremos", "habrá", "habrán", "habrás", 
    "habré", "habréis", "habría", "habríais", "habríamos", "habrían", "habrías", "han", 
    "has", "hasta", "hay", "haya", "hayamos", "hayan", "hayas", "hayáis", "he", "hemos", 
    "hube", "hubiera", "hubierais", "hubieran", "hubieras", "hubieron", "hubiese", 
    "hubieseis", "hubiesen", "hubieses", "hubimos", "hubiste", "hubisteis", "hubo", 
    "la", "las", "le", "les", "lo", "los", "me", "mi", "mis", "mucho", "muchos", "muy", 
    "más", "mí", "mía", "mías", "mío", "míos", "nada", "ni", "no", "nos", "nosotras", 
    "nosotros", "nuestra", "nuestras", "nuestro", "nuestros", "o", "os", "otra", "otras", 
    "otro", "otros", "para", "pero", "poco", "por", "porque", "que", "quien", "quienes", 
    "qué", "se", "sea", "seamos", "sean", "seas", "seremos", "será", "serán", "serás", 
    "seré", "seréis", "sería", "seríais", "seríamos", "serían", "serías", "seáis", "si", 
    "sido", "siendo", "sin", "sobre", "sois", "somos", "son", "soy", "su", "sus", "suya", 
    "suyas", "suyo", "suyos", "sí", "también", "tanto", "te", "tendremos", "tendrá", 
    "tendrán", "tendrás", "tendré", "tendréis", "tendría", "tendríais", "tendríamos", 
    "tendrían", "tendrías", "tened", "tenemos", "tenga", "tengamos", "tengan", "tengas", 
    "tengo", "tengáis", "tenida", "tenidas", "tenido", "tenidos", "teniendo", "tenéis", 
    "tenía", "teníais", "teníamos", "tenían", "tenías", "ti", "tiene", "tienen", "tienes", 
    "todo", "todos", "tu", "tus", "tuve", "tuviera", "tuvierais", "tuvieran", "tuvieras", 
    "tuvieron", "tuviese", "tuvieseis", "tuviesen", "tuvieses", "tuvimos", "tuviste", 
    "tuvisteis", "tuvo", "tuya", "tuyas", "tuyo", "tuyos", "tú", "un", "una", "uno", 
    "unos", "vosotras", "vosotros", "vuestra", "vuestras", "vuestro", "vuestros", "y", "ya", 
    "yo", "él", "éramos"
};

// English stop words (sorted and ready for binary search)
// Provided by deepseek
const char *ENGLISH_STOP_WORDS[] = {
    "a", "about", "above", "after", "again", "against", "all", "am", "an", "and",
    "any", "are", "aren't", "as", "at", "be", "because", "been", "before", "being",
    "below", "between", "both", "but", "by", "can", "can't", "cannot", "could",
    "couldn't", "did", "didn't", "do", "does", "doesn't", "doing", "don't", "down",
    "during", "each", "few", "for", "from", "further", "had", "hadn't", "has",
    "hasn't", "have", "haven't", "having", "he", "he'd", "he'll", "he's", "her",
    "here", "here's", "hers", "herself", "him", "himself", "his", "how", "how's",
    "i", "i'd", "i'll", "i'm", "i've", "if", "in", "into", "is", "isn't", "it",
    "it's", "its", "itself", "let's", "me", "more", "most", "mustn't", "my",
    "myself", "no", "nor", "not", "of", "off", "on", "once", "only", "or", "other",
    "ought", "our", "ours", "ourselves", "out", "over", "own", "same", "shan't",
    "she", "she'd", "she'll", "she's", "should", "shouldn't", "so", "some", "such",
    "than", "that", "that's", "the", "their", "theirs", "them", "themselves", "then",
    "there", "there's", "these", "they", "they'd", "they'll", "they're", "they've",
    "this", "those", "through", "to", "too", "under", "until", "up", "very", "was",
    "wasn't", "we", "we'd", "we'll", "we're", "we've", "were", "weren't", "what",
    "what's", "when", "when's", "where", "where's", "which", "while", "who", "who's",
    "whom", "why", "why's", "with", "won't", "would", "wouldn't", "you", "you'd",
    "you'll", "you're", "you've", "your", "yours", "yourself", "yourselves"
};

// Total number of of stopwords
const size_t NUMBER_OF_SPANISH_STOP_WORDS = sizeof(SPANISH_STOP_WORDS) / sizeof(SPANISH_STOP_WORDS[0]);
const size_t NUMBER_OF_ENGLISH_STOP_WORDS = sizeof(ENGLISH_STOP_WORDS) / sizeof(ENGLISH_STOP_WORDS[0]);

// Function to remove punctuation from tokens
void remove_punctuation_spanish(char *str) {
    // All the characters that we want to remove
    static const char punct[] = "$,.;:+-*/=¿?¡!()[]'\"°#&%";
     // The resultant token
    char *dst = str;
    // Character to parse
    unsigned char *ptr = (unsigned char *)str;
    
    while (*ptr) {
        // Handling special characters in UTF-8 (Á, É, Í, Ó, Ú, Ü, Ñ)
        // The bytes code was taken from deepseek
        if (*ptr == 0xC3) { // Byte used to advertise that a UTF-8 character is comming
            unsigned char first_byte = *ptr++;
            unsigned char second_byte = *ptr;
            
            // Convertir mayúsculas acentuadas a minúsculas
            switch(second_byte) {
                case 0x81: // Á
                    *dst++ = 0xC3; *dst++ = 0xA1; ptr++; break; // á
                case 0x89: // É
                    *dst++ = 0xC3; *dst++ = 0xA9; ptr++; break; // é
                case 0x8D: // Í
                    *dst++ = 0xC3; *dst++ = 0xAD; ptr++; break; // í
                case 0x93: // Ó
                    *dst++ = 0xC3; *dst++ = 0xB3; ptr++; break; // ó
                case 0x9A: // Ú
                    *dst++ = 0xC3; *dst++ = 0xBA; ptr++; break; // ú
                case 0x9C: // Ü
                    *dst++ = 0xC3; *dst++ = 0xBC; ptr++; break; // ü
                case 0x91: // Ñ
                    *dst++ = 0xC3; *dst++ = 0xB1; ptr++; break; // ñ
                default: // Passing another characters in UTF-8 that will not take into consideration
                    *dst++ = first_byte;
                    *dst++ = second_byte;
                    ptr++;
            }
        }
        // removing punctuation and changing to lower case
        else if (!strchr(punct, *ptr)) {
            *dst++ = tolower(*ptr++);
        }
        else {
            ptr++;
        }
    }
    *dst = '\0';
}

void remove_punctuation_english(char *str) {
    // All the characters that we want to remove
    static const char punct[] = "$,.;:+-*/=¿?¡!()[]\"°#&%";
     // The resultant token
    char *dst = str;
    // Character to parse
    unsigned char *ptr = (unsigned char *)str;
    
    while (*ptr) {
        // If the character is not in the characters that we want to remove
        if (!strchr(punct, *ptr)) {
            // parsing everything to lower case
            *dst++ = tolower(*ptr++);
        } else {
            ptr++;
        }
    }
    *dst = '\0';
}

// Main funtion: removing stop words
char* remove_stop_words_spanish(const char* line) {
    // null lines
    if (!line) return NULL;
    
    // Array to store the non-stop words
    char* result = calloc(MAX_SIZE, sizeof(char)); // calloc is the same than malloc but it initialized in 0
    if (!result) return NULL; // Memory problems
    
    // Copy of the line. It returns a pointer to the memory location
    char* rest = strdup(line);
    if (!rest) {
        free(result);
        return NULL;
    }

    // Tokenization
    char* token; // Array of strings/tokens
    char* saveptr; // Needed pointer to parse all the string
    token = strtok_r(rest, " \t\n\r", &saveptr);
    
    // Iterate over tokens
    while (token != NULL) {
        // Remove punctuation and putting into lower case
        remove_punctuation_spanish(token);

        // Check if the word is a stop word
        if (!is_stop_word_spanish(token)) {
            // Add the lower case token to the line
            strcat(result, token);
            strcat(result, " ");
        }
        // Get the next token
        token = strtok_r(NULL, " \t\n\r", &saveptr);
    }

    free(rest); // free memory of the copy
    return result;
}

// Main funtion: removing english stop words
char* remove_stop_words_english(const char* line) {
    // null lines
    if (!line) return NULL;
    
    // Array to store the non-stop words
    char* result = calloc(MAX_SIZE, sizeof(char)); // calloc is the same than malloc but it initialized in 0
    if (!result) return NULL; // Memory problems
    
    // Copy of the line. It returns a pointer to the memory location
    char* rest = strdup(line);
    if (!rest) {
        free(result);
        return NULL;
    }

    // Tokenization
    char* token; // Array of strings/tokens
    char* saveptr; // Needed pointer to parse all the string
    token = strtok_r(rest, " \t\n\r", &saveptr);
    
    // Iterate over tokens
    while (token != NULL) {
        // Remove punctuation and putting into lower case
        remove_punctuation_english(token);

        // Check if the word is a stop word
        if (!is_stop_word_english(token)) {
            // Add the lower case token to the line
            strcat(result, token);
            strcat(result, " ");
        }
        // Get the next token
        token = strtok_r(NULL, " \t\n\r", &saveptr);
    }

    free(rest); // free memory of the copy
    return result;
}

// Comparison function for binary search
int compare_strings(const void *a, const void *b) {
    const char *str_a = *(const char **)a;
    const char *str_b = *(const char **)b;
    return strcasecmp(str_a, str_b);
}

// Function to check if a word is a stop word using binary search
int is_stop_word_spanish(const char *word) {
    return bsearch(
        &word,
        SPANISH_STOP_WORDS, 
        NUMBER_OF_SPANISH_STOP_WORDS, 
        sizeof(const char*), 
        compare_strings  
    ) != NULL;
}

// Function to check if a word is a english stop word using binary search
int is_stop_word_english(const char *word) {
    return bsearch(
        &word,
        ENGLISH_STOP_WORDS, 
        NUMBER_OF_ENGLISH_STOP_WORDS, 
        sizeof(const char*), 
        compare_strings  
    ) != NULL;
}

// Function to sort the lines by index in ascending order
int comp_line_struct(const void *a, const void *b) {
    const Line *la = (const Line*)a;
    const Line *lb = (const Line*)b;
    return la->index - lb->index;
}