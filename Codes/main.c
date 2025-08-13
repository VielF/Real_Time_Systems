#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <windows.h>
#include <intrin.h>
#include <stdint.h>

#define BILLION 1000000000.0

#define _WIN64

// Windows
#ifdef _WIN64
#include <intrin.h>
#include <stdint.h>
uint64_t rdtsc(){
 return __rdtsc();
}
// Linux/GCC
#else
uint64_t rdtsc(){
 unsigned int lo,hi;
 __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
 return ((uint64_t)hi << 32) | lo;
}
#endif


void func1(){
    int x, y, z;

    y = 3;
    z = 4;
    x = y+z;
}

int func2(){
    int x, y, z;

    y = 3;
    z = 4;
    x = y+z;

    return x;
}

char func3(char param){
    int i;
    char x, y, z;
    for(i=0; i<1000;i++){
        if(param = 'a'){
            y = 6;
            z = 4;
            x = y+z;
        }else{
            y = 7;
            z = 5;
            x = y*z;
        }
        if(x == 10){
            //return 'a';
            param = 'b';
        }else{
            param = 'a';
        }
    }
    return 'a';
}


void func4(float param){
    printf("\n The parameter is: %.2f", param);
}

short func5(){
    int temp_param;
    printf("\nValue to temp_param will be 15: ");
    temp_param = 15;

    return ((short)temp_param);

}

int main(){
    char param_1 = 'b';
    short param_2;
    int param_3;
    float param_4 = 50.0;
    //clock_t begin, end;
    //double time_spent;

    //time_t begin, end;
    //double time_spent;

    //struct timespec start, end;
    //double time_spent;

    uint64_t begin, end, cycles_spent;

    func1();
    param_3 = func2();

    //begin = clock();
    //begin = time(NULL);
    //clock_gettime(CLOCK_REALTIME, &start);
    begin = rdtsc();
    param_1 = func3(param_1);
    //end = clock();
    //end = time(NULL);
    //clock_gettime(CLOCK_REALTIME, &end);
    end = rdtsc();


    //time_spent = (double)(end - begin)/CLOCKS_PER_SEC;
    //printf("Time: %f", time_spent);
    //time_spent = (end - begin);
    //printf("Time: %f", time_spent);
    //time_spent = (end.tv_sec - start.tv_sec) +  (end.tv_nsec - start.tv_nsec) / BILLION;
    //printf("Time: %f", time_spent);
    //printf("Time in ns: %f", (end.tv_nsec - start.tv_nsec));
    cycles_spent = (end - begin);
    printf("Cycles: %ld", cycles_spent);



    func4(param_4);
    param_2 = func5();
}
