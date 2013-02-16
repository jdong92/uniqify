/*
 * =====================================================================================
 *
 *       Filename:  uniqify.c
 *       Description:  Program that reads a textfile and print out the unique words
 *       Compiler:  icc
 *       Author:  John Dong
 *
 *       To run: ./uniqify [children] < textfile.txt
 *		
 * =====================================================================================
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#define SIZE 100

int main(int argc, char *argv[])

{
    int children = atoi(argv[1]);
    int i = 0;
    int word_to_sort[children][2];
    int sort_to_suppress[children][2]; 
    int childPID; 
    char word[SIZE];
    FILE *stream;

    for (i = 0; i < children; i++){

	    if (pipe(word_to_sort[children]) == -1){
		    perror("word_to_sort pipe error");
		    exit(-1);
	    }

        /*
        if (pipe(sort_to_suppress[children]) == -1){
            perror("sort_to_suppress pipe error");
            exit(-1);
        }
        */
    }


    switch(childPID = fork()){

        case -1:
            perror("error creating fork");
            exit(-1);
        case 0:
            dup2(word_to_sort[1][0], STDIN_FILENO);

            if (close(word_to_sort[1][1]) == -1)
                perror("error closing pipe");

            if (close(word_to_sort[1][0]) == -1)
                perror("error closing pipe");

            execl("/bin/sort", "sort", 0);
            break;

        default:
            break;


    }
    
    
	switch(childPID = fork()){
	
	case -1:
		fprintf(stderr, "error forking\n");
		break;

	case 0:
		break;

	default:

		if (close(word_to_sort[1][0]) == -1){
		    perror("perror closing pipe");
            exit(-1);
		}

		stream = fdopen(word_to_sort[1][1], "w");

		if (stream == NULL)
		    perror("error reading stream");

		if (close(word_to_sort[1][0]) == -1)
		    perror("error closing pipe");

		    
		while (fscanf(stdin, "%s", word) != EOF){

		    strcat(word, "\n");
		        //printf("%s \n", word);
		    fputs(word, stream);

		}
		   
		if (fclose(stream) == EOF)
		    perror("error closing stream");
		    break;
         
	}//end switch
 	    
}//end program
                 
