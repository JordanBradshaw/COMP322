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

#define THINKING 2
#define HUNGRY 1
#define EATING 0


//int N;

static int termcounter = 0;
int *nPointer;
int *counterPointer;
sem_t mutex;
sem_t *chopSticksPointer;
time_t seconds;
int *statePointer;
int *philPointer;
int *cyclePointer;
pid_t pid;
//usleep(usec);
void signalHandler(int);
int sigflag;

const char* signalReturnVal(int tempSignal) {//CONVERTING SIGNALS TO STRINGS
    if (tempSignal == 15) {
        return "SIGTERM";
    } else NULL;
}

void test(int phnum) {
    if (*(statePointer + phnum) == HUNGRY
            && (statePointer + (phnum+ 4) % nPointer) != EATING
            && (statePointer + (phnum + 1) % nPointer) != EATING) {
        // state that eating 
        *(statePointer + phnum) = EATING;
        //printf("Philosopher %d takes fork %d and %d\n", phnum + 1, LEFT + 1, phnum + 1);
        printf("Philosopher %d is eating.\n", phnum + 1);
        // sem_post(&S[phnum]) has no effect 
        // during takefork 
        // used to wake up hungry philosophers 
        // during putfork 
        //printf("%d\n", phnum);
        sem_post((chopSticksPointer + phnum));
        ++*(cyclePointer+phnum);
    }
}

void eat(int phnum) {
    sem_wait(&mutex);
    // state that hungry 
    *(statePointer + phnum) = HUNGRY;
    //printf("Philosopher %d is Hungry\n", phnum + 1);
    // eat if neighbours are not eating 
    test(phnum);
    sem_post(&mutex);
    // if unable to eat wait to be signalled 
    /*sem_wait(&S[phnum]);
    useconds_t usec = rand();
    printf("Eat Seconds: %d\n", usec);
    usleep(usec);*/
    sleep(1);
    //printf("Hi %d\n" , phnum);
}

void think(int phnum) {
    sem_wait(&mutex);
    // state that thinking 
    *(statePointer + phnum) = THINKING;
    /*printf("Philosopher %d putting fork %d and %d down\n",
            phnum + 1, LEFT + 1, phnum + 1);*/
    printf("Philosopher %d is thinking.\n", phnum + 1);
    test((phnum+ 4) % nPointer);
    test((phnum + 1) % nPointer);
    sem_post(&mutex);
    /*useconds_t usec = rand();
    printf("Think Seconds: %d\n", usec);
    usleep(usec);*/
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

void* philospher(void* num) {

    while (termcounter == 0) {
        //useconds_t msec = rand() % 1000000;
        //printf("%d\n", msec);
        int* i = num;
        usleep(rand() % 1000000);
        eat(*i);
        usleep(rand() % 1000000);
        think(*i);
    }
}

int main(int argc, char** argv) {
    int N = atoi(argv[1]);
    int counterArray[N];
    int state[N];
    sem_t chopSticks[N];
    int phil[N];
    int cycleArray[N];
    signal(SIGTERM, handler);
    nPointer = &N;
    counterPointer = counterArray;
    statePointer = state;
    chopSticksPointer = chopSticks;
    philPointer = phil;
    cyclePointer = cycleArray;
    for (int j = 0; j < N; j++) {
        printf("%d\n",N);
        *(counterPointer + j) = 0;
        *(philPointer + j) = j;
    }
    int i;
    pthread_t thread_id[N];
    fprintf(stderr, "PID: %d\n", getpid());
    // initialize the semaphores 
    sem_init(&mutex, 0, 1);
    for (i = 0; i < N; i++)
        sem_init((chopSticksPointer + i), 0, 0);
    for (i = 0; i < N; i++) {
        // create philosopher processes 
        pthread_create(&thread_id[i], NULL,
                philospher, &phil[i]);
    }
    for (i = 0; i < N; i++)
        pthread_join(thread_id[i], NULL);
    for (int j = 0; j < N; j++) {
        fprintf(stderr, "Philosopher %d has cycled %d times.\n", j + 1, cycleArray[j]);
    }
    return (EXIT_SUCCESS);
}