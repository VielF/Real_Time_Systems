#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define ESTAVEL 1
#define INSTAVEL 0

#ifndef _POSIX_THREAD_PRIO_INHERIT
#error "This system does not support priority inherit protection in mutex"
#endif

int sensor_duto= ESTAVEL;
int sensor_poco = ESTAVEL;

clock_t begin_global, end_global;

pthread_mutex_t lock;
pthread_mutexattr_t  mutexattr_prioinherit;

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

void init_mutex_inh();

void main(){
    pthread_t th_gas, th_oleo, th_poco;
    pthread_attr_t tattr;
    int newprio = 20;
    struct sched_param param;

    printf("\nSistemas de Monitoramento");

    pthread_mutex_init(&lock, NULL);

    init_mutex_inh();
    /* inicializado com atributos padrão */
    pthread_attr_init (&tattr);

    /* obter o parâmetro de programação existente */
    pthread_attr_getschedparam(&tattr, &param);

    /* definir a prioridade; outros parâmetros não mudaram */
    param.sched_priority = 5;

    /* definindo o novo parâmetro de escalonamento */
    pthread_attr_setschedparam (&tattr, &param);

    pthread_create(&th_gas, &tattr, thread_duto_gas, NULL);

    /* definir a prioridade; outros parâmetros não mudaram */
    param.sched_priority = 2;

    /* definindo o novo parâmetro de escalonamento */
    pthread_attr_setschedparam (&tattr, &param);

    pthread_create(&th_oleo, &tattr, thread_duto_oleo, NULL);


    /* definir a prioridade; outros parâmetros não mudaram */
    param.sched_priority = 6;

    /* definindo o novo parâmetro de escalonamento */
    pthread_attr_setschedparam (&tattr, &param);
    pthread_create(&th_poco, &tattr, thread_poco, NULL);

    while(1){
        usleep(10000);
    }
    pthread_join(th_gas, NULL);
    pthread_join(th_oleo, NULL);
    pthread_join(th_poco, NULL);


}

/* função para inicializar o mutex com o protocolo de herança de prioridade */
void init_mutex_inh(){
   int rtn;
   int mutex_protocol;

   /* Qual é o protocolo padrão no host? - retirado de exemplo*/
   if ((rtn = pthread_mutexattr_init(&mutexattr_prioinherit)) != 0)
      fprintf(stderr,"pthread_mutexattr_init: %s",strerror(rtn)); //mensagem em caso de haver erro
   if ((rtn = pthread_mutexattr_getprotocol(&mutexattr_prioinherit, &mutex_protocol)) != 0)
      fprintf(stderr,"pthread_mutexattr_getprotocol: %s",strerror(rtn)); //mensagem em caso de haver erro

#ifdef DEBUG 
   if (mutex_protocol == PTHREAD_PRIO_PROTECT)
      printf("Default mutex protocol is PTHREAD_PRIO_PROTECT\n");
   else if (mutex_protocol == PTHREAD_PRIO_INHERIT)
      printf("Default mutex protocol is PTHREAD_PRIO_INHERIT\n");
   else if (mutex_protocol == PTHREAD_PRIO_NONE)
      printf("Default mutex protocol is PTHREAD_PRIO_NONE\n");
   else 
      printf("Default mutex protocol is unrecognized: %d\n");
#endif

   /* Define o protocolo do mutex como INHERIT - herança de prioridade*/  
   if ((rtn = pthread_mutexattr_setprotocol(&mutexattr_prioinherit, PTHREAD_PRIO_INHERIT)) != 0)
    fprintf(stderr,"pthread_mutexattr_setprotocol: %s",strerror(rtn)); //mensagem em caso de haver erro

   /* Inicialize mutex com o objeto de atributo */
   if ((rtn = pthread_mutex_init(&lock,&mutexattr_prioinherit)) != 0)
      fprintf(stderr,"pthread_mutexattr_init: %s",strerror(rtn)); //mensagem em caso de haver erro

#ifdef DEBUG 
   /*verificação para identificar se o protocolo foi realmente atribuído */
   if ((rtn = pthread_mutexattr_getprotocol(&mutexattr_prioinherit, &mutex_protocol)) != 0)
      fprintf(stderr,"pthread_mutexattr_getprotocol: %s",strerror(rtn)); //mensagem em caso de haver erro

   if (mutex_protocol == PTHREAD_PRIO_PROTECT)
      printf("Default mutex protocol is PTHREAD_PRIO_PROTECT\n");
   else if (mutex_protocol == PTHREAD_PRIO_INHERIT)
      printf("Default mutex protocol is PTHREAD_PRIO_INHERIT\n");
   else if (mutex_protocol == PTHREAD_PRIO_NONE)
      printf("Default mutex protocol is PTHREAD_PRIO_NONE\n");
   else 
      printf("Default mutex protocol is unrecognized: %d\n");
#endif
}