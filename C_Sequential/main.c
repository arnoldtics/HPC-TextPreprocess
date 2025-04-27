#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "Timming.h"

int main(int argn, char **argv){
    if (argn > 2){
    	double utime0, stime0, wtime0, utime1, stime1, wtime1;
		uswtime(&utime0, &stime0, &wtime0);

        // Reading input file
        FILE *input = fopen(argv[1], "r");
        if (!input){
            printf("Error opening the file %s\n", argv[1]);
            return 1;
        }

        // Creating output file
        FILE *output = fopen(argv[2], "w");
        if (!output){
            printf("Error opening the file %s\n", argv[2]);
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

        uswtime(&utime1, &stime1, &wtime1);
		printf("\nBenchmarks (sec):\n");
	    printf("real %.3f\n", wtime1 - wtime0);
	    printf("user %.3f\n", utime1 - utime0);
	    printf("sys %.3f\n", stime1 - stime0);
	    printf("\n");
	    printf("CPU/Wall %.3f %% \n", 100.0 * (utime1 - utime0 + stime1 - stime0) / (wtime1 - wtime0));
    	printf("\n");
    } else {
        printf("Use: %s <input file> <output file>\n", argv[0]);
    }
}