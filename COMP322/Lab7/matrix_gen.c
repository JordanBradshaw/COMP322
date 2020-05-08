/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: yups
 *
 * Created on May 7, 2020, 6:22 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/aio.h>
#include <errno.h>

/*
 * 
 */
int main(int argc, char** argv) {
    struct aiocb cbOut;
    int fdOut = fileno(stdout);
    int num;
    int N = atoi(argv[1]);
    char buffer[N];
    if (fdOut != 0) {
        for (int j = 0; j < N; j++) {
            for (int i = 0; i < N; i++) {
                num = rand() % 200 - 100;
                printf("%d ", num);
            }
            printf("\n");
        }
    }
    bzero((char*) &cbOut, sizeof (struct aiocb));
    cbOut.aio_buf = buffer;
    cbOut.aio_fildes = fdOut;
    cbOut.aio_nbytes = N;
    int ret2 = aio_write(&cbOut);
    if (ret2 < 0) {
        fprintf(stderr, "AIO read failed");
    }
    while (aio_error(&cbOut) == EINPROGRESS) {
    }
    if ((ret2 = aio_return(&cbOut)) > 0) {
        fprintf(stderr, "Success");
    } else {
        fprintf(stderr, "Fail");
    }

    return (EXIT_SUCCESS);
}

