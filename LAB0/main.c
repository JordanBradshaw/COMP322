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
void DecodeText(int number) {
    printf("% 8d", ConvertD(number));
    printf("% 8c", ConvertD(number));
    printf("% 8s", getParity(ConvertD(number)) ? "odd" : "even");
    printf("\n");
}

int main(int argc, char** argv) {
    int number;
    char tempL[10000];
    printf("Enter the files location: \n");
    scanf("%s", tempL);
        printf("BINARY    DECIMAL   ASCII  PARITY\n");
    FILE *fileL = fopen(tempL, "r"); //read

    if (fileL == NULL) { //if file fails run this
        printf("Could not find file\n");
        exit(-1);
    }
    while (fscanf(fileL, "%d", & number) == 1) {
        int paddednumber;
        printf("%08d ", number);
       
        DecodeText(number);
    }

    return (EXIT_SUCCESS);
}

int ConvertD(int n) { //Convert from binary to decimal
    int convertD = 0, i = 0, remainder;
    while (n != 0) {
        remainder = n % 10;
        n /= 10;
        convertD += remainder * pow(2, i);
        ++i;
    }
    return convertD;

}

bool getParity(unsigned int n) {
    bool parity = 0;
    while (n) {
        parity = !parity;
        n = n & (n - 1);
    }
    return parity;
}

