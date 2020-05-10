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
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <aio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
const int N = 1000;
struct aiocb cbNext;
struct aiocb cbPrev;
struct aiocb cbCurr;
int CURRENT, last, next;
int retCurrRead, retCurrWrite, retPrevWrite, retNext, retNextRead, retNextWrite;



volatile void *buffCur;
#define TOTAL (size*size*4)
#define BLOCKSIZE (size*size*4)/blocks
#define READ 0
#define WRITE 1

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

void loadIn(struct aiocb *temp, size_t bytes, off_t off) {
    memset(temp, 0, sizeof (struct aiocb));
    temp->aio_fildes = fileno(stdin);
    temp->aio_nbytes = bytes;
    temp->aio_offset = off;
    temp->aio_buf = malloc(bytes);
    temp->aio_reqprio = 0;
}

void loadOut(struct aiocb *temp, size_t bytes, off_t off) {
    temp->aio_fildes = fileno(stdout);
    temp->aio_nbytes = bytes;
    temp->aio_offset = off;
}

void matrixCalc(int scalar, int size, int blocks) {//the lack of organization was irritating
    int CURRENT = 0;
    fprintf(stderr, "TotalSize: %d\n", TOTAL);
    fprintf(stderr, "BlockSize: %d\n", BLOCKSIZE);
    //////////////////////////////////////ORIGINAL LOAD (KEEP)
    loadIn(&cbCurr, BLOCKSIZE, 0);
    retCurrRead = aio_read(&cbCurr);
    if (retCurrRead < 0) {
        fprintf(stderr, "AIO CURR READ FAIL");
    }
    while (aio_error(&cbCurr) == EINPROGRESS) {
    }
    if ((retCurrRead = aio_return(&cbCurr)) > 0) {
        //fprintf(stderr, "Success\n");
    }
    ////////////////////////////////////// END OF PRE LOOP
    /////////////////////////////////////////////////////////////////////
    for (CURRENT = BLOCKSIZE; CURRENT < TOTAL; CURRENT += BLOCKSIZE) {
        fprintf(stderr, "BLOCKSIZE = %d \n", CURRENT);
        //next = current + BLOCKSIZE;
        /////////////////////////////////////NEXT LOAD (IN LOOP))
        loadIn(&cbNext, CURRENT, BLOCKSIZE);
        retNextRead = aio_read(&cbNext);
        if (retNextRead < 0) {
            fprintf(stderr, "AIO READ NEXT FAIL");
        }
        while (aio_error(&cbNext) == EINPROGRESS) {
        }
        if ((retNextRead = aio_return(&cbNext)) > 0) {
            //fprintf(stderr, "Success\n");
        } else {
            int errnum;
            fprintf(stderr, "Fail Second Read %d\n", errno);
            perror("PRINT ERROR");
        }///////////////////////////////////////// MATRIX ADD

        ////////////////////////////////////////  WRITE PREV
        loadOut(&cbPrev, CURRENT, BLOCKSIZE);
        cbPrev.aio_buf = cbCurr.aio_buf;
        fprintf(stderr, "%s", cbPrev.aio_buf);
        retPrevWrite = aio_write(&cbPrev);
        if (retPrevWrite < 0) {
            fprintf(stderr, "AIO PREV WRITE failed");
        }
        while (aio_error(&cbPrev) == EINPROGRESS) {
        }
        if ((retPrevWrite = aio_return(&cbPrev)) > 0) {
            //fprintf(stderr, "Success\n");
        } else {
            int errnum;
            fprintf(stderr, "Fail %d <~~", errno);
            perror("PRINT ERROR");
        }/////////////////////////// SYNC OUT MOVE NEXT TO CURR
        aio_fsync(O_SYNC, &cbPrev);
        memcpy(&cbCurr, &cbNext, sizeof (struct aiocb));
    }////////////////////////////////////////////END OF FOR LOOP
    /////////////////////////////////////////////////
    /////////////////////////////////////////////CURRENT FINAL WRITE
    loadOut(&cbCurr, BLOCKSIZE, CURRENT);
    retCurrWrite = aio_write(&cbCurr);
    if (retCurrWrite < 0) {
        fprintf(stderr, "AIO CURR WRITE failed");
    }
    while (aio_error(&cbCurr) == EINPROGRESS) {
    }
    if ((retCurrWrite = aio_return(&cbCurr)) > 0) {
        //fprintf(stderr, "Success\n");
    } else {
        fprintf(stderr, "Fail");
    }
    aio_fsync(O_SYNC, &cbCurr);
    /////////////////////////////END OF MATRIX CALC

}

int main(int argc, char** argv) {
    clock_t begin = clock();
    int size = atoi(argv[1]), blocks = atoi(argv[2]);
    int scaler = rand() % 100;
    matrixCalc(scaler, size, blocks);

    clock_t end = clock();
    double totalTime = (double) (end - begin) / CLOCKS_PER_SEC;
    return (EXIT_SUCCESS);
}

