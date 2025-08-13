#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define ESTAVEL 1
#define INSTAVEL 0

int sensor_duto_gas = ESTAVEL;
int sensor_duto_oleo = ESTAVEL;
int sensor_poco = ESTAVEL;


void *thread_duto_gas(void *arg){
    int aleatoridade = 0;
    
    while(1){
        aleatoridade = (rand()%1000);
        if(aleatoridade >= 0 && aleatoridade <= 2){
            sensor_duto_gas = INSTAVEL;
            printf("\nSensor do duto de gas instável");
            printf("\nAplicação de contra medida.");
            printf("\nAplicação de contra medida..");
            printf("\nAplicação de contra medida...");
            printf("\nContra medida concluida");
        }else{
            printf("\nSensor do duto de gas estável");
            sensor_duto_gas = ESTAVEL;
        }
        usleep(10000);
    }

}

void *thread_duto_oleo(void *arg){
    int aleatoridade = 0;
    
    while(1){
        aleatoridade = (rand()%100);
        if(aleatoridade >= 200 && aleatoridade <= 202){
            sensor_duto_oleo = INSTAVEL;
            printf("\nSensor do duto de oleo instável");
            printf("\nAplicação de contra medida.");
            printf("\nAplicação de contra medida..");
            printf("\nAplicação de contra medida...");
            printf("\nContra medida concluida");
        }else{
            printf("\nSensor do duto de oleo estável");
            sensor_duto_oleo = ESTAVEL;
        }
        usleep(10000);
    }

}

void *thread_poco(void *arg){
    int aleatoridade = 0;
    
    while(1){
        aleatoridade = (rand()%100);
        if(aleatoridade >= 700 && aleatoridade <= 702){
            sensor_poco = INSTAVEL;
            printf("\nSensor do poço instável");
            printf("\nAplicação de contra medida.");
            printf("\nAplicação de contra medida..");
            printf("\nAplicação de contra medida...");
            printf("\nContra medida concluida");
        }else{
            printf("\nSensor do poço estável");
            sensor_poco = ESTAVEL;
        }
        usleep(10000);
    }

}

void main(){
    pthread_t th_gas, th_oleo, th_poco;
    printf("\nSistemas de Monitoramento");
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