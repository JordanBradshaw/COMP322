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
#include <aio.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
struct aiocb cbNext;
struct aiocb cbPrev;
struct aiocb cbCurr;
int CURRENT, last, next;
int retCurrRead, retCurrWrite, retPrevWrite, retNext, retNextRead, retNextWrite;
#define TOTAL (size*size*4)            //4K Page Size (8*8*4) = 256
#define OFFSET (blockSize*blockSize*12)//2^12 = 4K   (8/2)^2*12 = 192
#define BLOCKSIZE blockSize //WANTED COLOR CLARITY
#define LAST TOTAL-BLOCKSIZE
#define NEXT CURRENT+blockSize
#define PREV CURRENT//-blockSize
size_t blockSize = 0;

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

void loadIn(struct aiocb *temp, off_t off) {
    memset(temp, 0, sizeof (struct aiocb));
    temp->aio_fildes = fileno(stdin);
    temp->aio_nbytes = blockSize;
    temp->aio_offset = off;
    temp->aio_buf = malloc(blockSize);
    temp->aio_reqprio = 0;
}

void loadOut(struct aiocb *temp, off_t off) {
    temp->aio_fildes = fileno(stdout);
    temp->aio_nbytes = blockSize;
    temp->aio_offset = off;
}

void matrixCalc(int scalar, int size, int blocks) {//the lack of organization was irritating
    blockSize = (size * size * 4) / blocks;
    fprintf(stderr, "TotalSize: %d\n", TOTAL);
    fprintf(stderr, "BlockSize: %ld\n", blockSize);
    //////////////////////////////////////ORIGINAL LOAD (KEEP)
    loadIn(&cbCurr, 0);
    retCurrRead = aio_read(&cbCurr);
    if (retCurrRead < 0) {
        fprintf(stderr, "AIO CURR READ FAIL (1)");
    }
    while (aio_error(&cbCurr) == EINPROGRESS) {
    }
    aio_return(&cbCurr);
    ////////////////////////////////////// END OF PRE LOOP
    for (CURRENT = BLOCKSIZE; CURRENT < TOTAL; CURRENT += BLOCKSIZE) {
        /////////////////////////////////////NEXT LOAD (IN LOOP))
        loadIn(&cbNext, NEXT);
        retNextRead = aio_read(&cbNext);
        if (retNextRead < 0) {
            fprintf(stderr, "AIO READ NEXT FAIL (2)");
        }
        while (aio_error(&cbNext) == EINPROGRESS) {
        }
        aio_return(&cbNext);
        ///////////////////////////////////////// MATRIX ADD

        ////////////////////////////////////////  WRITE PREV
        memcpy(&cbPrev, &cbCurr, sizeof (struct aiocb));
        loadOut(&cbPrev, CURRENT);
        fprintf(stderr, "%s", cbPrev.aio_buf);
        retPrevWrite = aio_write(&cbPrev);
        if (retPrevWrite < 0) {
            fprintf(stderr, "AIO PREV WRITE FAIL (3)\n");
        }
        while (aio_error(&cbPrev) == EINPROGRESS) {
        }
        if ((retPrevWrite = aio_return(&cbPrev)) > 0) {
            fprintf(stderr, "Success\n");
        } else {
            int errnum;
            fprintf(stderr, "Fail %d <~~\n", errno);
            perror("PRINT ERROR");
        }/////////////////////////// SYNC OUT MOVE NEXT TO CURR
        if (aio_fsync(O_SYNC, &cbPrev) != -1) {
            fprintf(stderr, "\nSync success\n");
        }
        memcpy(&cbCurr, &cbNext, sizeof (struct aiocb));
    }////////////////////////////////////////////END OF FOR LOOP
    /////////////////////////////////////////////////
    /////////////////////////////////////////////CURRENT FINAL WRITE
    loadOut(&cbCurr, LAST);
    retCurrWrite = aio_write(&cbCurr);
    if (retCurrWrite < 0) {
        fprintf(stderr, "AIO CURR WRITE FAIL (4)");
    }
    while (aio_error(&cbCurr) == EINPROGRESS) {
    }
    retCurrWrite = aio_return(&cbCurr);
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

