/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: yups
 *
 * Created on May 1, 2020, 4:02 PM
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>   /* umask() */
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
static pid_t mole1;
static pid_t mole2;
static int num;
int status1;
int status2;


void randomMoleChoose() {
srand(time(NULL));///<~~~~ THIS IS WHY rand() was always returning 1
    char mNum[30];
    num = rand() % 2;
    snprintf(mNum,2 ,"%d", num + 1);
    char *mArgv[] = {"mole", mNum, 0};
    if (num == 0){
        if (kill(mole1,SIGCHLD) < 0){
            fprintf(stderr,"already dead");
        }
        waitpid(mole1,&status1,0);
        if(WIFEXITED(status1)){
        mole1 = fork();
        if (mole1 == 0){
        execv(mArgv[0], mArgv);}}
    }else {
        if (kill(mole2,SIGCHLD) < 0){
            fprintf(stderr,"already dead");
        }
         waitpid(mole2,&status2,0);
         if(WIFEXITED(status2)){
        mole2 = fork();
        if (mole2 == 0){
        execv(mArgv[0], mArgv);}
        }
        }}
    


static void handler(int signum) {
    if (signum == SIGTERM) {
        kill(mole1, SIGTERM);
        kill(mole2, SIGTERM);
        exit(0);
    } else if (signum == SIGUSR1) {
        randomMoleChoose();
        (void) signal(SIGUSR1, handler);
    } else if (signum == SIGUSR2) {
        randomMoleChoose();
        (void) signal(SIGUSR2, handler);
    }
}

int main(int argc, char **argv) {

    pid_t daemonPID;
    pid_t sid;
    if ((daemonPID = fork()) < 0)
        fprintf(stderr, "fork() failed\n");
    if (daemonPID != 0) {
        // Kill parent so daempn runs in background
        printf("Daemon PID: %d\n", daemonPID);
        exit(0);
    }
    if (daemonPID == 0) {
        signal(SIGTERM, handler); //REDIRECT SIGNALS TO HANDLER
        signal(SIGUSR1, handler);
        signal(SIGUSR2, handler);
        umask(0); //Change file permissions of daemon
        if ((sid = setsid()) < 0) //make daemon session leader
            fprintf(stderr, "setsid() failed\n");
        int fd1 = open("/dev/null", O_RDONLY);
        if (chdir("/") != 0) //CHANGE DIRECTORY TO ROOT
            fprintf(stderr, "chdir() failed\n");
        if (fd1 < 0) //ERROR
            fprintf(stderr, "failed to open log\n");
        if (dup2(fd1, STDOUT_FILENO) < 0) //REDIRECT STDOUT TO LOG FILE
            fprintf(stderr, "redirect STDOUT failed");
        if (dup2(fd1, STDERR_FILENO) < 0) //REDIRECT STDERR TO LOG FILE
            fprintf(stderr, "redirect STDERR failed\n");
        if (dup2(fd1, STDIN_FILENO) < 0)
            fprintf(stderr, "redirect STDIN failed\n");
        close(fd1); //CLOSE FILE
        while (1) {
            sleep(1);
        }
    }

    return (0);
}

