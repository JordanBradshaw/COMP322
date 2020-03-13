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

/*
 * 
 */
int main(int argc, char** argv) {
    int pid = fork(); //FORK AND DECLARE VARS
    int childStatus = -1;
    pid_t wPID;
    time_t startSeconds, endSeconds;

    bool validPID = (pid >= 0) ? true : false;
    if (validPID == true) {
        if (pid == 0) {//PID RETURNS 0 IF IN CHILD

            exit(0); //CLOSE WHEN FINISHED
        } else {//PID RETURNS PID OF CHILD (IN PARENT)

            wPID = waitpid(pid, &childStatus, WUNTRACED); //WAIT FOR CHILD PROCESS TO STOP
            fprintf(stderr, "%d", validPID);
        }
    } else {//VALID PID == FALSE SO!
        printf("Error Creating Child!");
    }

    return (EXIT_SUCCESS);
}

