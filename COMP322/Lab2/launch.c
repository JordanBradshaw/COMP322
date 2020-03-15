/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: yups
 *
 * Created on March 12, 2020, 7:45 PM
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
    pid_t pid = fork(); //FORK AND DECLARE VARS
    char *newArg[argc];
    int childStatus = -1;
    pid_t wPID;
    bool validPID = (pid >= 0) ? true : false;
    if (validPID == true) {
        if (pid == 0) {//PID RETURNS 0 IF IN CHILD
            int i, k = 0;
            //printf("%d", argc);
            for (i = 1; i < argc; i++) {
                newArg[k] = argv[i];
                k++;
                //printf("arv[]: %s\n", argv[i]);
            }
            newArg[k] = NULL;
            char *newargv[] = {NULL, "-ls", "-ls", NULL};
            char *newenviron[] = {NULL};
            if (argc != 2) {
                fprintf(stderr, "Usage: %s <file-to-exec>\n", argv[0]);
                exit(EXIT_FAILURE);
            }
            newargv[0] = argv[1];
            execve(newArg[0], newArg, newenviron);
            perror("execve");
            exit(0); //CLOSE WHEN FINISHED
        } else {//PID RETURNS PID OF CHILD (IN PARENT)
            fprintf(stderr, "Child PID: %d\n", pid);
            wPID = waitpid(pid, &childStatus, WUNTRACED); //WAIT FOR CHILD PROCESS TO STOP
            fprintf(stderr, "Child Return Value: %d\n", convertByte2Bit(childStatus));
        }
    } else {//VALID PID == FALSE SO!
        printf("Error Creating Child!");
    }

    return (EXIT_SUCCESS);
}

