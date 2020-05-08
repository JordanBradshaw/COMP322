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

/*
 * 
 */
int main(int argc, char** argv) {
    FILE *fp = fopen("matrix.txt", "w+");
    int num;
    //int i;
    int N = atoi(argv[1]);
    if (fp == NULL) {
        fprintf(stderr, "Error creating file");
    }
    if (fp != 0) {
        for (int j = 0; j < N; j++) {
            for (int i = 0; i < N; i++) {
                num = rand()%200 - 100;
                fprintf(fp, "%d ", num);
            }
            fprintf(fp, "\n");
        }
    }

    return (EXIT_SUCCESS);
}

