#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "functions.h"

int PrimeCount(int A, int B){
    int n = B + 1;
    int prime[n];
    for(int i = 0; i < n; ++i){
        prime[i] = 1;
    }
    for (int i = 2; i*i <= B; ++i)
	    if (prime[i])
            for(int j=i*i; j <= B; j+=i)
                prime[j] = 0;
    int count = 0;
    for(int i = A; i < B; ++i){
        count += prime[i]? 1:0;
    }
    return count;
}

double my_log(double num, double base){
    return log2(num)/log2(base);
}

char* Translation(long x){
    char *result = NULL;
    int size = floor(my_log(x,3)) + 2;
    result = malloc(sizeof(char) * size);
    result[size - 1] = '\0';
    while(x > 0){
        --size;
        result[size - 1] = (x % 3) + '0';
        x /= 3;
    }
    return result;
}