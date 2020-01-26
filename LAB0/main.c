/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: yups
 *
 * Created on January 25, 2020, 8:05 PM
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * 
 */
int main(int argc, char** argv) {
    char tempL[MAX_LEN];
    printf("Enter File Location");
    scanf ("%s", tempL);
    FILE *in_file = fopen(tempL, "r"); //read
    FILE *out_file = fopen(tempL, "w"); // write
    
    if (in_file == NULL || out_file == NULL){
        printf("Could not find file");
        exit(-1);
    }
    
    
    return (EXIT_SUCCESS);
}

