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
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#define LEFT (phil->current % phil->total)
#define RIGHT (phil->current+1 % phil->total)
#define THINKING 2
#define HUNGRY 1
#define EATING -1

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
    sem_t** stix;
};

const char* signalReturnVal(int tempSignal) {//CONVERTING SIGNALS TO STRINGS
    if (tempSignal == 15) {
        return "SIGTERM";
    } else NULL;
}

void eat(struct thinker * phil) {
    if (sem_wait(phil->stix[LEFT]) != EATING && (sem_wait(phil->stix[RIGHT]) != EATING)) {
        printf("Philosopher %d is eating.\n", phil->current);
        usleep(rand() % 1000000);
    }
    
}

void think(struct thinker * phil) {
    if (sem_post(phil->stix[LEFT]) != EATING && (sem_post(phil->stix[RIGHT]) != EATING)) {
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
    char stixBuffer[20];
    phil->total = atoi(argv[1]);
    phil->current = atoi(argv[2]);
    phil->stix = malloc(sizeof (sem_t *) * phil->total);
    for (int i = 0; i < phil->total; ++i) {
        memset(stixBuffer, 0, 20);
        sprintf(stixBuffer, "chopStickNum%d", i);
        phil->stix[i] = sem_open(&stixBuffer, O_CREAT, 0644, 1);
        printf("Chopstick%d\n", i);
    }
}

int main(int argc, char** argv) {
    struct thinker phil;
    philBuild(argv, &phil);
    signal(SIGTERM, handler);
    philCycle(&phil);
    fprintf(stderr, "Philosopher %d has cycled %d times.\n", phil.current, phil.cycles);
    return (EXIT_SUCCESS);
}