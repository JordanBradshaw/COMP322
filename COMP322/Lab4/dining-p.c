/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: yups
 *
 * Created on April 17, 2020, 2:59 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define THINKING 2
#define HUNGRY 1
#define EATING -1
#define LEFT (phil->current % phil->total)
#define RIGHT ((phil->current+1) % phil->total)

static int termcounter = 0;
sem_t mutex;
sem_t *chopSticksPointer;
time_t seconds;
int *statePointer;
pid_t pid;
void signalHandler(int);
int sigflag;

struct thinker {
    int total;
    int current;
    int cycles;
    int left;
    int right;
    sem_t** stix;
};

const char* signalReturnVal(int tempSignal) {//CONVERTING SIGNALS TO STRINGS
    if (tempSignal == 15) {
        return "SIGTERM";
    } else NULL;
}

void eat(struct thinker * phil) {
    if (sem_wait(phil->stix[LEFT]) != EATING
            && sem_wait(phil->stix[RIGHT]) != EATING) {
        perror("hi");
        printf("Philosopher %d is eating.\n", phil->current);
        usleep(rand() % 1000000);
    }
}

void think(struct thinker * phil) {
    if (sem_post(phil->stix[LEFT]) != EATING &&
            sem_post(phil->stix[RIGHT]) != EATING) {
        printf("Philosopher %d is thinking.\n", phil->current);
        usleep(rand() % 1000000);
    }
}

void handler(int signo) {//NEW HANDLER
    time(&seconds);
    signal(signo, handler);
    if (signo == SIGTERM) {
        termcounter++;
        printf("%s received at %ld\n", signalReturnVal(signo), seconds);
    } else {
    }
}

void philCycle(struct thinker * phil) {
    while (termcounter == 0) {
        eat(phil);
        think(phil);
        phil->cycles++;
    }
}

void philBuild(char** argv, struct thinker * phil) {
    char stixBuffer[30];
    phil->total = atoi(argv[1]);
    phil->current = atoi(argv[2]);
    phil->stix = malloc(sizeof (sem_t *) * phil->total);
    for (int i = 0; i < phil->total; ++i) {
        memset(stixBuffer, 0, 30);
        sprintf(stixBuffer, "chopStickNum%d", i);
        phil->stix[i] = sem_open(stixBuffer, O_CREAT, 0644, 1);
        //printf("Chopstick%d\n", i);
    }
    phil->left = LEFT;
    phil->right = RIGHT;
}

void breakStix(struct thinker * phil) {
    char stixBuffer[30];
    for (int i = 0; i < phil->total; ++i) {
        if (sem_close(phil->stix[i]) == -1) {
            perror("Chopstick Close");
        }
        memset(stixBuffer, 0, 30);
        sprintf(stixBuffer, "chopStickNum%d", i);
    }
    if (sem_unlink(stixBuffer) == -1) {
        //perror("ChopStick unlink error");
    }
}

void groupFile() {
    FILE *fptr;
    struct stat buf;
    char* line = NULL;
    size_t len = 0;
    if (stat("groupID.txt", &buf) == 0) {
        fptr = fopen("groupID.txt", "r");
        getline(&line, &len, fptr);
        if (setpgid(getpid(), strtol(line, NULL, 10)) != 0) {
            perror("setpgid() error");
        }
        fclose(fptr);
    } else {
        char buffer[32];
        memset(buffer, '\0', 32);
        fptr = fopen("groupID.txt", "w");
        sprintf(buffer, "%d", getpgid(getpid()));
        fwrite(buffer, sizeof (char), sizeof (buffer), fptr);
        fclose(fptr);
    }
}

void structCheck(struct thinker * phil) {
    printf("Current:%d Total:%d Left:%d Right:%d\n", phil->current, phil->total, phil->left, phil->right);
}

int main(int argc, char** argv) {
    struct thinker phil;
    printf("PID: %d\n", getpid());
    groupFile();
    philBuild(argv, &phil);
    //leftRightCalculate(&phil);
    signal(SIGTERM, handler);
    structCheck(&phil);
    philCycle(&phil);
    breakStix(&phil);
    fprintf(stderr, "Philosopher %d has cycled %d times.\n", phil.current, phil.cycles);
    return (EXIT_SUCCESS);
}