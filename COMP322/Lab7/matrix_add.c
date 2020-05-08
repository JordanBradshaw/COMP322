/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: yups
 *
 * Created on May 7, 2020, 6:46 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/aio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
const int SIZE_TO_READ = 1000;
struct aiocb cbIn;
struct aiocb cbOut;

/*
 * 
 */
void matrix_add(int block, int size, int scalar) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            //block[i][j] += scalar;
        }
    }

}

void zeroOut(struct aiocb temp) {
    temp.aio_buf = 0;
    temp.aio_fildes = 0;
    temp.aio_lio_opcode = 0;
    temp.aio_nbytes = 0;
    temp.aio_offset = 0;
    temp.aio_reqprio = 0;
}

int main(int argc, char** argv) {
    clock_t begin = clock();
    int ret;
    int ret2;
    char buffer[SIZE_TO_READ];
    int fdOut = fileno(stdout);
    int fdIn = fileno(stdin);
    bzero((char*) &cbIn, sizeof (struct aiocb));
    cbIn.aio_buf = malloc(SIZE_TO_READ + 1);
    cbIn.aio_fildes = fdIn;
    cbIn.aio_nbytes = SIZE_TO_READ;
    cbIn.aio_offset = 0;
    ret = aio_read(&cbIn);
    if (ret < 0) {
        fprintf(stderr, "AIO read failed");
    }
    while (aio_error(&cbIn) == EINPROGRESS) {
    }
    if ((ret = aio_return(&cbIn)) > 0) {
        printf("Success");
    } else {
        printf("Fail");
    }

    bzero((char*) &cbOut, sizeof (struct aiocb));
    cbOut.aio_buf = cbIn.aio_buf;
    cbOut.aio_fildes = fdOut;
    cbOut.aio_nbytes = SIZE_TO_READ;
    ret2 = aio_write(&cbOut);
    if (ret2 < 0) {
        fprintf(stderr, "AIO read failed");
    }
    while (aio_error(&cbOut) == EINPROGRESS) {
    }
    if ((ret2 = aio_return(&cbOut)) > 0) {
        printf("Success");
    } else {
        printf("Fail");
    }






    int scaler = rand();
    int size = atoi(argv[1]);

    clock_t end = clock();
    double totalTime = (double) (end - begin) / CLOCKS_PER_SEC;
    return (EXIT_SUCCESS);
}

