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
#include <sys/stat.h>
#include <fcntl.h>

#define SIZE 25
#define BUF_SIZE 1024

void filter_punctuation(char *s);

int main(int argc, char *argv[])

{
    int children = atoi(argv[1]);
    int i, j, k, l, status;
    int num_empty = 0;
    int empty[children];
    char previous[SIZE];
    char current[children];
    int word_to_sort[children][2];
    int sort_to_suppress[children][2]; 
    char word[SIZE];
    char text[children][BUF_SIZE];
    FILE *(wstream[children]); //word_to_sort stream
    FILE *(sstream[children]); //sort_to_suppressor

    for (i = 0; i < children; i++){

	    if (pipe(word_to_sort[i]) == -1){
		    perror("word_to_sort pipe creation error");
	    } 
 
        if (pipe(sort_to_suppress[i]) == -1){
            perror("sort_to_suppress pipe creation error");
        }

        empty[i] = 0;
    }

    for (i = 0; i < children; i++){

        switch(fork()){

            case -1:
                perror("error creating fork");
                exit(-1);

            case 0:
                for (j = 0; j < children; j++){

                    if (j != i){

                        if (close(word_to_sort[j][0]) == -1)
                            perror("word_to_sort[0] error closing pipe"); 
                        
                        if (close(sort_to_suppress[j][1]) == -1)
                            perror("word_to_suppress[0] error closing pipe");

                    }else{

                        dup2(word_to_sort[j][0], STDIN_FILENO);
                        dup2(sort_to_suppress[j][1], STDOUT_FILENO);

                        if (close(word_to_sort[j][0]) == -1)
                            perror("word_to_sort[0] error closing pipe");

                        if (close(sort_to_suppress[j][1]) == -1)
                            perror("sort_to_suppress[0] error closing pipe");
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

	switch(fork()){
	
		case 0:
	    /*Most of the suppressor code I got help from Michael Phan who took this class last term. */
		
		for (i = 0; i < children; i++){     
		    close(sort_to_suppress[i][1]);
		    close(word_to_sort[i][0]);
		    close(word_to_sort[i][1]);
		    sstream[i] = fdopen(sort_to_suppress[i][0], "r");
		    fgets(text[i], BUF_SIZE, sstream[i]);            
		}

		while (num_empty < children){

		    j = 0;
		    while((empty[j] == 1) && (j < children)){
		        j++;
		    }

		    for (i = j; i < children; i++){
		        if((strcmp(text[i], text[j]) < 0) && (empty[i] == 0)){
		            j = i;
		            strcpy(current, text[j]);
		        }else{
		            if (empty[j] == 0){
		                strcpy(current, text[j]);
		            }
		        }

		    }

		    if (strcmp(previous, current) != 0){
		        printf("%s", current);
		        strcpy(previous, current);
		    }

		    if (fgets(text[j], BUF_SIZE, sstream[j]) == NULL){
		        empty[j] = 1;
		    }

		    num_empty = 0;
		    k = 0; 
		    while(k < children){

		        if (empty[k] == 1){
		            num_empty++;
		        }
		        k++;
		    }
		}

		default:
		for (i = 0; i < children; i++){

			close(word_to_sort[i][0]);
		    close(sort_to_suppress[i][1]);
		    close(sort_to_suppress[i][0]);
		    wstream[i] = fdopen(word_to_sort[i][1], "w");
		}       
		i = 0;
			while (fscanf(stdin, "%s", word) != EOF){
		       
		    if (wstream[i] == NULL)
				perror("error reading stream");	
		    
		    filter_punctuation(word);
		    strcat(word, "\n");
		    
		    if (i >= children - 1){
		        i = 0;
		    }

		    fputs(word, wstream[i]);
		    i++;
			}	   
		    for (i = 0; i < children; i++){
		    fclose(wstream[i]);
		}

		for (i = 0; i < children; i++){
		    wait(&status);
		}
		wait(&status);
         
	}//end switch
 	    
}//end program

/* Function source: http://stackoverflow.com/questions/9283555/removing-punctuations-in-a-string-in-c */
void filter_punctuation(char *s){

    for (char *p = s; *p; ++p)
        if (isalpha(*p))
            *s++ = tolower(*p);
    *s = '\0';
}
                 
