/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Tube.c
 * Author: yups
 *
 * Created on March 12, 2020, 10:04 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "unistd.h"
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * 
 */

convertByte2Bit(int tempByte) {//BIT SHIFT TO GET VALUE
    return ((tempByte >> 0) & 1);
}

int main(int argc, char** argv) {
    int childStatusA = -1;
    int childStatusB = -1;
    pid_t wPIDA, wPIDB;
    char buffer;
    int pipeA[2];
    int cVar = 0;
    for (int c = 1; c < argc; c++) {
        if (strcmp(argv[c], ",") == 0) {
            cVar = c;
            //printf("Cvar: %d\n", cVar);
            break;
        }
    }
    char *argVar1[cVar];
    int l = 0;
    for (int i = 1; i < cVar; i++) {
        argVar1[i] = argv[i];
        //printf("argVar1[%d] %s\n", l, argVar1[i]);
        l++;
    }
    char *argVar2[argc - cVar];
    int j = 0;
    for (int k = cVar + 1; k < argc; k++) {
        argVar2[j] = argv[k];
        //printf("argVar2[%d] %s\n", j, argv[k]);
        j++;
    }
    argVar1[l] = NULL;
    argVar2[j] = NULL;
    //printf("%d : %d\n", l , j);
    if (pipe(pipeA) == -1) {
        perror("Building pipes failed");
        exit(EXIT_FAILURE);
    }

    pid_t pidA = fork();
    if (pidA == 0) {//childA
        close(pipeA[0]); //CLOSE READ
        dup2(pipeA[1], STDOUT_FILENO); //DUP WRITE OF PIPE TO STDOUT
        if (execve(argVar1[0], argVar1, NULL) == -1) {
            printf("Child 1 Error\n");
        }
        //printf("This is line 1st line\n");
        //close(pipeA[1]);
        //execlp(argv[0], argVar1, NULL);
        //fprintf(stderr, "Failed to execute '%s'\n", argv[1]);
        //wait(NULL);
        exit(0); //CLOSE WHEN FINISHED
    } else if (pidA > 0) {
        pid_t pidB = fork();
         if (pidB == 0) {//childB
            close(pipeA[1]); //CLOSE WRITING
            dup2(pipeA[0], STDIN_FILENO); //DUP READ TO PIPE TO STDIN
            if (execve(argVar2[0], argVar2, NULL) == -1) {
                printf("Child 2 Error\n");
            }
            //fprintf(stderr, "This is line 2nd line\n");
            //fprintf(stderr, "Failed to exe '%s'\n", args2[0]);
            exit(0);
        }
        if (pidB > 0) {
            fprintf(stderr, "Child A: PID: %d\n", pidA);
            fprintf(stderr, "Child B: PID: %d\n", pidB);
            close(pipeA[0]); //CLOSE READING END PIPEA
            close(pipeA[1]); //CLOSE WRITING END PIPEA
            wPIDA = waitpid(pidA, &childStatusA, 0); //WAIT FOR CHILD PROCESS A TO STOP
            wPIDB = waitpid(pidB, &childStatusB, 0); //WAIT FOR CHILD PROCESS B TO STOP
            int exitStatusA = WEXITSTATUS(childStatusA);
            int exitStatusB = WEXITSTATUS(childStatusB);
            fprintf(stderr, "Exit Status A:%d\n", convertByte2Bit(exitStatusA));
            fprintf(stderr, "Exit Status B:%d\n", convertByte2Bit(exitStatusB));
            exit(0);
        }


    }

    return (EXIT_FAILURE);
}

