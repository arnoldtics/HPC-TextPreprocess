#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

int main(int argn, char **argv){
    if (argn > 1){
        // Reading input file
        FILE *input = fopen(argv[1], "r");
        if (!input){
            printf("Error opening the file %s\n", argv[1]);
            return 1;
        }

        // Creating output file
        FILE *output = fopen("output.txt", "w");
        if (!output){
            printf("Error opening the file output.txt\n");
            fclose(input);
            return 1;
        }

        // printf("'el' es stop word? %d\n", is_stop_word("el"));  // Debe imprimir 1 (true)
        // printf("'zorro' es stop word? %d\n", is_stop_word("zorro"));  // Debe imprimir 0 (false)

        Line *lines = NULL; // Dynamic array for all lines
        char buffer[MAX_SIZE]; // Buffer for reading each line
        int idx = 0; // Index: the numbre of line

        while(fgets(buffer, MAX_SIZE, input)){
            // Redimension of the array dynamically (append each line)
            Line *temp = realloc(lines, (idx+1)*sizeof(Line));
            if (!temp){
                perror("Memory error\n");
                break;
            }
            lines = temp;

            // Removing stop words from the line
            char* cleaned_line = remove_stop_words(buffer);
            if (!cleaned_line){
                perror("Error while cleaning the line\n");
                break;
            }

            // Saving the line
            lines[idx].index = idx;
            lines[idx].line = cleaned_line; 

            if (!lines[idx].line){
                perror("Error while copying the line\n");
                break;
            }
            idx++;
        }

        // Sorting with the index of each line
        qsort(lines, idx, sizeof(Line), comp_line_struct);
    
        for(int i = 0; i < idx; i++){
            fprintf(output, "%s\n", lines[i].line);
            free(lines[i].line); // returning memory of each line
        }

        free(lines); // returning memory of all the array
        fclose(input);
        fclose(output);
    } else {
        printf("You must write the file name as an argument\n");
    }
}