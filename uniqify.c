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
    int i, j, status;
    int word_to_sort[children][2];
    int sort_to_suppress[children][2]; 
    int childPID; 
    char word[SIZE];
    FILE *(stream[children]);

    for (i = 0; i < children; i++){

	    if (pipe(word_to_sort[i]) == -1){
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

    for (i = 0; i < children; i++){

        switch(childPID = fork()){

            case -1:
                perror("error creating fork");
                exit(-1);
            case 0:
                for (j = 0; j < children; j++){

                    if (j != i){

                        if (close(word_to_sort[j][0]) == -1)
                            perror("error closing pipe");

                    }else{

                        dup2(word_to_sort[j][0], STDIN_FILENO);
              
                        if (close(word_to_sort[j][0]) == -1)
                            perror("error closing pipe");
                    }
                    close(word_to_sort[j][1]);

                }
                
                execl("/bin/sort", "sort", 0);
                _exit(EXIT_SUCCESS);

            default:
                break;
        }
    }
     
	switch(childPID = fork()){
	
	case -1:
		fprintf(stderr, "error forking\n");
		break;

	case 0:
        //suppressor goes here
		break;

	default:

        for (i = 0; i < children; i++){

		    if (close(word_to_sort[i][0]) == -1){
		        perror("perror closing pipe");
                exit(-1);    
            } 

            stream[i] = fdopen(word_to_sort[i][1], "w");
        }	
		    
		while (fscanf(stdin, "%s", word) != EOF){

            i = 0;
            if (stream[i] == NULL)
		        perror("error reading stream");	
		   
            strcat(word, "\n");
		    //printf("%s \n", word);
            
            if (i >= children - 1){
                i = 0;
            }

		    fputs(word, stream[i]);
            i++;
		}
		   
		if (fclose(stream[i]) == EOF)
		    perror("error closing stream");
		    break;

        for (i = 0; i < children; i++){
            wait(&status);
        }
         
	}//end switch
 	    
}//end program
                 
