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
#define BUFSIZE 100

int main(int argc, char *argv[])

{
<<<<<<< HEAD
	int pipefd[children][2];
	int childPID; 
   
 	char word[BUFSIZE];
    FILE *stream;

	if (pipe(pipefd) == -1){

		perror("pipe error");
		exit(-1);
	}

	switch(childPID = fork()){
	
        case -1:
            fprintf(stderr, "error forking\n");
            break;

        case 0:
            dup2(pipefd[0], STDIN_FILENO);
            //dup2(sort2suppress, STDOUT_FILENO);

            if (close(pipefd[1]) == -1)
                perror("error closing pipe");

            if (close(pipefd[0]) == -1)
                perror("error closing pipe");

            execl("/bin/sort", "sort", 0);
            exit(EXIT_SUCCESS);
            break;

        default:  
	        close(pipefd[0]);

            stream = fdopen(pipefd[1], "w");

            if (stream == NULL)
                perror("error reading stream");

            if (close(pipefd[0]) == -1)
                perror("error closing pipe");

            
            while (fscanf(stdin, "%s", word) != EOF){

                strcat(word, "\n");
                //printf("%s \n", word);
                fputs(word, stream);

            }
           
            if (fclose(stream) == EOF)
                perror("error closing stream");
            break;
         
	}
    
   
=======
        int     fd[2], nbytes;
        int     result; 
        char    readBuffer[BUFSIZE];

        if(pipe(fd) != 0){
                perror("Pipes");
                exit(-1);
        }

        switch((result = fork())){

            case -1:
                    break;

            case 0:
                    close(fd[1]);
                    //write to the content of fd[0] to STDOUT
                    while ((nbytes = read(fd[0], readBuffer, BUFSIZE)) > 0){
                        if (write(STDOUT_FILENO, readBuffer, nbytes) == -1)
                            perror("write");
                    }

            default:
                    close(fd[0]);
                    //read the STDIN and write to fd[1]
                    while ((nbytes = read(STDIN_FILENO, readBuffer, BUFSIZE)) > 0){
                        if (write(fd[1], readBuffer, nbytes) == -1)
                            perror("write");
                    }           

        }
>>>>>>> 8afe5df649866c94419630f950a7291e68a6475c

}
