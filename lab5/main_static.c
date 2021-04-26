#include <stdio.h>
#include "functions.h"



int main(){
    int sw = -1;
    while(scanf("%d",&sw) > 0){
        if(sw == 1){
            int A, B;
            scanf("%d",&A);
            scanf("%d",&B);            
            printf("%d\n",PrimeCount(A,B));
        } else if(sw == 2) {
            long x = 0;
            scanf("%ld",&x);
            printf("%s\n",Translation(x));
        }
    }
    return 0;
}
