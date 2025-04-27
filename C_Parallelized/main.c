#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "common.h"
#include "Timming.h"

int main(int argn, char **args) {
    int my_proc, num_procs;
	double utime0, stime0, wtime0, utime1, stime1, wtime1;

    MPI_Status status;
    
    MPI_Init(&argn, &args);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_proc);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    
    if (my_proc == 0) { // Master
        if (argn < 4) {
            printf("Use: %s <input file> <output file> <english/spanish>\n", args[0]);
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 1;
        }
        
		uswtime(&utime0, &stime0, &wtime0);

        // Input file (Count the number of lines)
        FILE *input = fopen(args[1], "r");
        if (!input) {
            printf("Error with input file %s\n", args[1]);
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 1;
        }

        unsigned long long line_count = 0;
        char buffer[MAX_SIZE];
        
        while (fgets(buffer, MAX_SIZE, input)) {
            line_count++;
            if (line_count >= MAX_STATIC_LINES) {
                printf("Warning: File too large, processing only first %d lines\n", MAX_STATIC_LINES);
                break;
            }
        }
        
        // If the file is empty
        if (line_count == 0) {
            printf("Empty file or error reading file\n");
            fclose(input);
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 1;
        }
        
        // Dynamic memory allocation
        MPILine *lines = (MPILine*)malloc(line_count * sizeof(MPILine));
        if (!lines) {
            printf("Memory allocation failed\n");
            fclose(input);
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 1;
        }
        
        // Return to the start and read all the lines
        rewind(input);
        unsigned long long current_line = 0;
        while (fgets(buffer, MAX_SIZE, input) && current_line < line_count) {
            lines[current_line].index = current_line; // index control to keep order
            lines[current_line].original_line = strdup(buffer); // raw lines (just the master)
            if (!lines[current_line].original_line) {
                printf("Memory error at line %llu\n", current_line);
                break;
            }
            lines[current_line].cleaned_line = NULL; // clean lines (workers)
            // lines[current_line].processed = 0; // process flag, free
            current_line++;
        }
        fclose(input);
        
        printf("Master: Read %llu lines\n", line_count);
        
        // Output file
        FILE *output = fopen(args[2], "w");
        if (!output) {
            printf("Error with output file %s\n", args[2]);
            free(lines);
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 1;
        }

        // Verify spanish or english files
        int language;
        if(strcmp(args[3], "spanish") == 0){language = 1;}
        else if(strcmp(args[3], "english") == 0){language = 0;}
        else{
            printf("Error detecting the language: %s\nenglish or spanish are only available\n", args[3]);
            free(lines);
            fclose(output);
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 1;
        }
        
        // Sending to the workers
        unsigned long long next_line = 0;
        unsigned long long completed_lines = 0;
        
        // Sending first chunk of lines
        for (int worker = 1; worker < num_procs && next_line < line_count; worker++) {
            MPI_Send(&lines[next_line].index, 1, MPI_UNSIGNED_LONG_LONG, worker, 0, MPI_COMM_WORLD);
            MPI_Send(lines[next_line].original_line, strlen(lines[next_line].original_line)+1, MPI_CHAR, worker, 0, MPI_COMM_WORLD);
            MPI_Send(&language, 1, MPI_INT, worker, 0, MPI_COMM_WORLD);
            //lines[next_line].processed = -1;
            next_line++;
        }
        
        // Barber: receiving and sending
        while (completed_lines < line_count) {
            char cleaned_line[MAX_SIZE];
            unsigned long long received_index;
            // Master receiving cleaned lines
            MPI_Recv(&received_index, 1, MPI_UNSIGNED_LONG_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Recv(cleaned_line, MAX_SIZE, MPI_CHAR, status.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            // Saving the new line
            lines[received_index].cleaned_line = strdup(cleaned_line);

            if (!lines[received_index].cleaned_line) {
                printf("Memory error storing cleaned line %llu\n", received_index);
                lines[received_index].cleaned_line = strdup("ERROR");
            }

            // Flag control (processed)
            //lines[received_index].processed = 1;
            completed_lines++;
            
            // Sending new line to the worker
            if (next_line < line_count) {
                MPI_Send(&lines[next_line].index, 1, MPI_UNSIGNED_LONG_LONG, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
                MPI_Send(lines[next_line].original_line, strlen(lines[next_line].original_line)+1, MPI_CHAR, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
                MPI_Send(&language, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
                //lines[next_line].processed = -1; // flag in progress
                next_line++;
            } else {
                // Sending notification of termination
                unsigned long long terminate = -1;
                MPI_Send(&terminate, 1, MPI_UNSIGNED_LONG_LONG, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
            }
        }
        
        // Writing output
        for (unsigned long long i = 0; i < line_count; i++) {
            if (lines[i].cleaned_line) {
                fprintf(output, "%s\n", lines[i].cleaned_line);
            }
            free(lines[i].original_line);
            free(lines[i].cleaned_line);
        }
        fclose(output);
        free(lines);
        
        printf("Master: Ready!\n");
        
    } else { // Workers
        while (1) {
            // First the worker receives the line number or the flag for ending work
            unsigned long long line_index;
            MPI_Recv(&line_index, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            
            // If the number is a flag, it have finished the work
            if (line_index == -1) break;
            
            // If not, it is the index of a new line to process
            char original_line[MAX_SIZE];
            
            // Receiving the line
            MPI_Recv(original_line, MAX_SIZE, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            // Receiving the flag for language
            int language_flag;
            MPI_Recv(&language_flag, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            
            // Main function: removing the stop words
            char* cleaned_line = NULL;
            if(language_flag){cleaned_line = remove_stop_words_spanish(original_line);}
            else{cleaned_line = remove_stop_words_english(original_line);}

            if (!cleaned_line) {
                cleaned_line = strdup("Error at the process\n");
            }
            
            // Sending the cleaned line back to the master
            MPI_Send(&line_index, 1, MPI_UNSIGNED_LONG_LONG, 0, 0, MPI_COMM_WORLD);
            MPI_Send(cleaned_line, strlen(cleaned_line)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            
            free(cleaned_line);
        }
    }

    // MPI_Barrier(MPI_COMM_WORLD);
	if (my_proc == 0){
		uswtime(&utime1, &stime1, &wtime1);
		printf("\nBenchmarks (sec):\n");
	    	printf("real %.3f\n", wtime1 - wtime0);
	    	printf("user %.3f\n", utime1 - utime0);
	    	printf("sys %.3f\n", stime1 - stime0);
	    	printf("\n");
	    	printf("CPU/Wall %.3f %% \n", 100.0 * (utime1 - utime0 + stime1 - stime0) / (wtime1 - wtime0));
	    	printf("\n");
	}
    
    MPI_Finalize();
    return 0;
}
