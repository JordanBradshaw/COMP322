/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   moles.c
 * Author: yups
 *
 * Created on May 5, 2020, 7:59 PM
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {
    FILE *fp = fopen("lab6.log", "a");
    if (fp == NULL){
        fprintf(stderr, "Lab creation failed");
    }
    if (fp != 0) {
        char *mole;
        if (argc > 1 && strcmp(argv[1], "1") == 0)
            mole = "Pop mole1";
        else
            mole = "Pop mole2";
        fclose(fp);
    }
    return 0;
}

