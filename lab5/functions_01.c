#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "functions.h"

int PrimeCount(int A, int B){
    int n = B + 1;
    int flag, count = 0;
    for(int i = A; i < n; ++i){
        flag = 1;
        for(int j = i - 1; j >= 2; --j){
            if(i % j == 0){
                flag = 0;
            }
        }
    count+=flag;
    }    
    return count;
}

char* Translation(long x){
    char* result = NULL;
    int size = floor(log2(x)) + 2;
    result = malloc(sizeof(char) * size);
    result[size - 1] = '\0';
    while(x > 0){
        --size;
        result[size - 1] = (x % 2) + '0';
        x /= 2;
    }
    return result;
}