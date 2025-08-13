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

pthread_mutex_t lock;

void *thread_duto_gas(void *arg){
    int aleatoridade = 0;
    
    while(1){
        aleatoridade = (rand()%100);
        if(aleatoridade >= 0 && aleatoridade <= 2){
            pthread_mutex_lock(&lock);
            sensor_duto = INSTAVEL;
            printf("\nSensor do duto de gas instável");
            printf("\nAplicação de contra medida.");
            printf("\nAplicação de contra medida..");
            printf("\nAplicação de contra medida...");
            printf("\nContra medida concluida");
            pthread_mutex_unlock(&lock);
        }else{
            pthread_mutex_lock(&lock);
            sensor_duto = ESTAVEL;
            pthread_mutex_unlock(&lock);
        }
        usleep(10000);
    }

}

void *thread_duto_oleo(void *arg){
    int aleatoridade = 0;
    
    while(1){
        aleatoridade = (rand()%100);
        if(aleatoridade >= 30 && aleatoridade <= 32){
            pthread_mutex_lock(&lock);
            sensor_duto = INSTAVEL;
            printf("\nSensor do duto de oleo instável");
            printf("\nAplicação de contra medida.");
            printf("\nAplicação de contra medida..");
            printf("\nAplicação de contra medida...");
            printf("\nContra medida concluida");
            pthread_mutex_unlock(&lock);
        }else{
            pthread_mutex_lock(&lock);
            sensor_duto = ESTAVEL;
            pthread_mutex_unlock(&lock);
        }
        usleep(10000);
    }

}

void *thread_poco(void *arg){
    int aleatoridade = 0;
    
    while(1){
        aleatoridade = (rand()%100);
        if(aleatoridade >= 70 && aleatoridade <= 72){
            sensor_poco = INSTAVEL;
            pthread_mutex_lock(&lock);
            printf("\nSensor do poço instável");
            printf("\nAplicação de contra medida.");
            printf("\nAplicação de contra medida..");
            printf("\nAplicação de contra medida...");
            printf("\nContra medida concluida");
            pthread_mutex_unlock(&lock);
        }else{
            sensor_poco = ESTAVEL;
        }
        usleep(10000);
    }

}

void main(){
    pthread_t th_gas, th_oleo, th_poco;
    printf("\nSistemas de Monitoramento");
    pthread_mutex_init(&lock, NULL);

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