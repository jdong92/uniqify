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
    FILE *(wstream[children]); //word_to_sort stream
    FILE *(sstream[children]); //sort_to_suppressor

    for (i = 0; i < children; i++){

	    if (pipe(word_to_sort[i]) == -1){
		    perror("word_to_sort pipe error");
		    exit(-1);
	    }

        if (pipe(sort_to_suppress[children]) == -1){
            perror("sort_to_suppress pipe error");
            exit(-1);
        }
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

                        if (close(sort_to_suppress[j][1]) == -1)
                            perror("error closing pipe");

                    }else{

                        dup2(word_to_sort[j][0], STDIN_FILENO);
                        dup2(sort_to_suppress[j][1], STDOUT_FILENO);

                        if (close(word_to_sort[j][0]) == -1)
                            perror("error closing pipe");

                        if (close(sort_to_suppress[j][1]) == -1)
                            perror("error closing pipe");
                    }

                    close(word_to_sort[j][1]);
                    close(sort_to_suppress[j][0]);

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
        for (i = 0; i < children; i++){
            
            if (i != j){

                sstream[j] = fdopen(pipeds[1], "r");

            }else{

            }
            close(sort_to_suppress[i][1]);
        }
		break;

	default:

        for (i = 0; i < children; i++){

		    if (close(word_to_sort[i][0]) == -1){
		        perror("perror closing pipe");
                exit(-1);    
            } 

            wstream[i] = fdopen(word_to_sort[i][1], "w");
        }	
		    
		while (fscanf(stdin, "%s", word) != EOF){

            i = 0;
            if (wstream[i] == NULL)
		        perror("error reading stream");	
		   
            strcat(word, "\n");
		    //printf("%s \n", word);
            
            if (i >= children - 1){
                i = 0;
            }

		    fputs(word, wstream[i]);
            i++;
		}
		   
		if (fclose(wstream[i]) == EOF)
		    perror("error closing stream");
		    break;

        for (i = 0; i < children; i++){
            wait(&status);
        }
         
	}//end switch
 	    
}//end program
                 
