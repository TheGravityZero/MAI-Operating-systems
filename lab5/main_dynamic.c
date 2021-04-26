#include <dlfcn.h>
#include <stdio.h>


int main(){
    int (*PrimeCount)(int, int) = NULL;
    char* (*Translation)(long) = NULL;
    int sw = -1;
    char* libs[] = {"lib1.so","lib2.so"};
    int lib = 0;
    void* handle = NULL;
    handle = dlopen(libs[lib],RTLD_LAZY);
    if (!handle) {
        printf("%s\n", dlerror());
        return 1;
    }
    printf("%p \n", handle);
    PrimeCount = dlsym(handle,"PrimeCount");
    Translation = dlsym(handle,"Translation");
    printf("%p %p \n", PrimeCount, Translation);
    while(scanf("%d",&sw) > 0){
        if(sw == 1){
            int A, B;
            scanf("%d %d",&A, &B);
            printf("%d\n",(PrimeCount)(A,B));
        } else if(sw == 2) {
            long x = 0;
            scanf("%ld",&x);
            printf("%s\n",Translation(x));
        } else if(sw == 0) {
            dlclose(handle);
            lib = (lib + 1) % 2;
            handle = dlopen(libs[lib],RTLD_LAZY);
            PrimeCount = dlsym(handle,"PrimeCount");
            Translation = dlsym(handle,"Translation");
        }
    }
    dlclose(handle);
}