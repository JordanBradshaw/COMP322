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
#include <math.h>
#include<string.h>
#define bool int

/*
 * 
 */


int ConvertD(char *n) { //Convert from binary to decimal
    int convertD = 0, i = 0, remainder;
    int numberConv = atoi(n);
    while (numberConv != 0) {
        remainder = numberConv % 10;
        numberConv /= 10;
        convertD += remainder * pow(2, i);
        ++i;
    }
    return convertD;

}

bool getParity(int n) {
    bool parity = 0;
    while (n) {
        parity = !parity;
        n = n & (n - 1);
    }
    return parity;
}

void DecodeText(char *number) {
    printf("% 8s", number);
    printf("% 8d", ConvertD(number));
    printf("% 8c", ConvertD(number));
    printf("% 8s", getParity(ConvertD(number)) ? "odd" : "even");
    printf("\n");
}

int main(int argc, char** argv) {
    char number[20];
    char tempL[10000];
    printf("Enter the files location: \n");
    scanf("%s", tempL);
    printf("BINARY   DECIMAL   ASCII  PARITY\n");
    FILE *fileL = fopen(tempL, "r"); //read

    if (fileL == NULL) { //if file fails run this
        printf("Could not find file\n");
        exit(-1);
    }
    while (fscanf(fileL, "%s", number) == 1) {
        char buffer[9];
        int digits;
        snprintf(buffer, 9, "%s", number);
        digits = strlen(buffer);

        for (int i = digits; i < 8; i++) {
            buffer[i] = '0';
        }
        DecodeText(buffer);
    }

    return (EXIT_SUCCESS);
}



