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

//int N = 5;
#define N 5
#define LEFT (phnum+ 4) % N
#define RIGHT (phnum + 1) % N
#define THINKING 2
#define HUNGRY 1
#define EATING 0


//int N;

static int termcounter = 0;
int counterArray[N];
sem_t mutex;
sem_t chopSticks[N];
time_t seconds;
int state[N];
int phil[N];
pid_t pid;
//usleep(usec);
void signalHandler(int);
int sigflag;
int cycleArray[N];

const char* signalReturnVal(int tempSignal) {//CONVERTING SIGNALS TO STRINGS
    if (tempSignal == 15) {
        return "SIGTERM";
    } else NULL;
}

void test(int phnum) {

    if (state[phnum] == HUNGRY
            && state[LEFT] != EATING
            && state[RIGHT] != EATING) {
        // state that eating 
        state[phnum] = EATING;
        //printf("Philosopher %d takes fork %d and %d\n", phnum + 1, LEFT + 1, phnum + 1);
        printf("Philosopher %d is eating.\n", phnum + 1);
        // sem_post(&S[phnum]) has no effect 
        // during takefork 
        // used to wake up hungry philosophers 
        // during putfork 
        //printf("%d\n", phnum);
        sem_post(&chopSticks[phnum]);
        cycleArray[phnum]++;
    }
}

void eat(int phnum) {
    sem_wait(&mutex);
    // state that hungry 
    state[phnum] = HUNGRY;
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
    state[phnum] = THINKING;
    /*printf("Philosopher %d putting fork %d and %d down\n",
            phnum + 1, LEFT + 1, phnum + 1);*/
    printf("Philosopher %d is thinking.\n", phnum + 1);
    test(LEFT);
    test(RIGHT);
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
    //N = 5;
    signal(SIGTERM, handler);
    /*phil = (int*) malloc(N * sizeof (int));
    cycleArray = (int*) malloc(N * sizeof (int));
    counterArray = (int*) malloc(N * sizeof (int));
    state = (int*) malloc(N * sizeof (int));
    phil = (int*) malloc(N * sizeof (int));
    chopSticks = (sem_t*) malloc(N * sizeof (sem_t));*/

    for (int j = 0; j < N; j++) {
        counterArray[j] = 0;
        phil[j] = j;
    }
    int i;
    pthread_t thread_id[N];
    fprintf(stderr, "PID: %d\n", getpid());
    // initialize the semaphores 
    sem_init(&mutex, 0, 1);
    for (i = 0; i < N; i++)
        sem_init(&chopSticks[i], 0, 0);
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