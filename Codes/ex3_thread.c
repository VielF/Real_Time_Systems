#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define ESTAVEL 1
#define INSTAVEL 0

int sensor_duto= ESTAVEL;
int sensor_poco = ESTAVEL;

clock_t begin_global, end_global;

pthread_mutex_t lock_1, lock_2, lock_3;

void *thread_duto_gas(void *arg){
    int aleatoridade = 0;
    clock_t begin_local, end_local;
    while(1){
        begin_local = clock();
        aleatoridade = (rand()%100);
        pthread_mutex_lock(&lock_1);
        if(aleatoridade >= 0 && aleatoridade <= 2){
            //pthread_mutex_lock(&lock_1);
            sensor_duto = INSTAVEL;
            printf("\nSensor do duto de gas instável");
            printf("\nAplicação de contra medida.");
            printf("\nAplicação de contra medida..");
            printf("\nAplicação de contra medida...");
            printf("\nContra medida concluida");
         //   pthread_mutex_lock(&lock_1);
        }else{
         //   pthread_mutex_lock(&lock_1);
            sensor_duto = ESTAVEL;
         //   pthread_mutex_unlock(&lock_1);
        }
        end_local = clock();
        pthread_mutex_lock(&lock_2);
        end_global = clock();
        pthread_mutex_unlock(&lock_2);
        //pthread_mutex_unlock(&lock_1);
        printf("\nTime: to execute duto_gas: %lf", (double)(end_local - begin_local));
        pthread_mutex_lock(&lock_3);
        printf("\nMomento global - duto gas: %lf", (double)(end_global - begin_global));
        pthread_mutex_unlock(&lock_3);
        pthread_mutex_unlock(&lock_1);
        usleep(10000);
    }

}

void *thread_duto_oleo(void *arg){
    int aleatoridade = 0;
    clock_t begin_local, end_local;
    while(1){
        begin_local = clock();
        aleatoridade = (rand()%100);
        pthread_mutex_lock(&lock_1);
        if(aleatoridade >= 30 && aleatoridade <= 32){
            //pthread_mutex_lock(&lock_1);
            sensor_duto = INSTAVEL;
            printf("\nSensor do duto de oleo instável");
            printf("\nAplicação de contra medida.");
            printf("\nAplicação de contra medida..");
            printf("\nAplicação de contra medida...");
            printf("\nContra medida concluida");
            //pthread_mutex_unlock(&lock_1);
        }else{
            //pthread_mutex_lock(&lock_1);
            sensor_duto = ESTAVEL;
            //pthread_mutex_unlock(&lock_1);
        }
        end_local = clock();
        pthread_mutex_lock(&lock_2);
        end_global = clock();
        pthread_mutex_unlock(&lock_2);
        printf("\nTime: to execute duto_oleo: %lf", (double)(end_local - begin_local));
        pthread_mutex_lock(&lock_3);
        printf("\nMomento global - duto oleo: %lf", (double)(end_global - begin_global));
        pthread_mutex_unlock(&lock_3);
        pthread_mutex_unlock(&lock_1);
        usleep(10000);
    }

}

void *thread_poco(void *arg){
    int aleatoridade = 0;
    clock_t begin_local, end_local;
    while(1){
        begin_local = clock();
        aleatoridade = (rand()%100);
        pthread_mutex_lock(&lock_1);
        if(aleatoridade >= 70 && aleatoridade <= 72){
            sensor_poco = INSTAVEL;
            //pthread_mutex_lock(&lock_1);
            printf("\nSensor do poço instável");
            printf("\nAplicação de contra medida.");
            printf("\nAplicação de contra medida..");
            printf("\nAplicação de contra medida...");
            printf("\nContra medida concluida");
            //pthread_mutex_unlock(&lock_1);
        }else{
            sensor_poco = ESTAVEL;
        }
        end_local = clock();
        pthread_mutex_lock(&lock_2);
        end_global = clock();
        pthread_mutex_unlock(&lock_2);
        //pthread_mutex_unlock(&lock_1);
        printf("\nTime: to execute poco: %lf", (double)(end_local - begin_local));
        pthread_mutex_lock(&lock_3);
        printf("\nMomento global - poco: %lf", (double)(end_global - begin_global));
        pthread_mutex_unlock(&lock_3);
        pthread_mutex_unlock(&lock_1);
        usleep(10000);
    }

}

void main(){
    pthread_t th_gas, th_oleo, th_poco;
    printf("\nSistemas de Monitoramento");

    pthread_mutex_init(&lock_1, NULL);
    pthread_mutex_init(&lock_2, NULL);
    pthread_mutex_init(&lock_3, NULL);

    begin_global = clock();
    pthread_create(&th_gas, NULL, thread_duto_gas, NULL);
    pthread_create(&th_oleo, NULL, thread_duto_oleo, NULL);
    pthread_create(&th_poco, NULL, thread_poco, NULL);

    while(1){
        usleep(10000);
    }
    pthread_join(th_gas, NULL);
    pthread_join(th_oleo, NULL);
    pthread_join(th_poco, NULL);


}