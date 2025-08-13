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
pthread_mutexattr_t  mutexattr_prioinherit;
int high_prio;

void *thread_duto_gas(void *arg){
    int aleatoridade = 0;
    
    while(1){
        aleatoridade = (rand()%100);
        if(aleatoridade >= 0 && aleatoridade <= 2){
            if((rtn = pthread_mutex_lock( &lock)) != 0){ /*Trava a seção critica - pode não conseguir por causa do teto de prioridade*/
                fprintf(stderr,"\npthread_mutex_lock: %s",strerror(rtn)); //mensagem em caso de haver erro
            }else{
                sensor_duto = INSTAVEL;
                printf("\nSensor do duto de gas instável");
                printf("\nAplicação de contra medida.");
                printf("\nAplicação de contra medida..");
                printf("\nAplicação de contra medida...");
                printf("\nContra medida concluida");
                pthread_mutex_unlock(&lock);
            }
        }else{
            if((rtn = pthread_mutex_lock( &lock)) != 0){ /*Trava a seção critica - pode não conseguir por causa do teto de prioridade*/
                fprintf(stderr,"\npthread_mutex_lock: %s",strerror(rtn)); //mensagem em caso de haver erro
            }else{
                sensor_duto = ESTAVEL;
                pthread_mutex_unlock(&lock);
            }
        }
        usleep(10000);
    }

}

void *thread_duto_oleo(void *arg){
    int aleatoridade = 0;
    
    while(1){
        aleatoridade = (rand()%100);
        if(aleatoridade >= 30 && aleatoridade <= 32){
            if((rtn = pthread_mutex_lock( &lock)) != 0){ /*Trava a seção critica - pode não conseguir por causa do teto de prioridade*/
                fprintf(stderr,"\npthread_mutex_lock: %s",strerror(rtn)); //mensagem em caso de haver erro
            }else{
                sensor_duto = INSTAVEL;
                printf("\nSensor do duto de oleo instável");
                printf("\nAplicação de contra medida.");
                printf("\nAplicação de contra medida..");
                printf("\nAplicação de contra medida...");
                printf("\nContra medida concluida");
                pthread_mutex_unlock(&lock);
            }
        }else{
            if((rtn = pthread_mutex_lock( &lock)) != 0){ /*Trava a seção critica - pode não conseguir por causa do teto de prioridade*/
                fprintf(stderr,"\npthread_mutex_lock: %s",strerror(rtn)); //mensagem em caso de haver erro
            }else{
                sensor_duto = ESTAVEL;
                pthread_mutex_unlock(&lock);
            }
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
            if((rtn = pthread_mutex_lock( &lock)) != 0){ /*Trava a seção critica - pode não conseguir por causa do teto de prioridade*/
                fprintf(stderr,"\npthread_mutex_lock: %s",strerror(rtn)); //mensagem em caso de haver erro
            }else{
                printf("\nSensor do poço instável");
                printf("\nAplicação de contra medida.");
                printf("\nAplicação de contra medida..");
                printf("\nAplicação de contra medida...");
                printf("\nContra medida concluida");
                pthread_mutex_unlock(&lock);
            }
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
    struct sched_param param;
    int thread_id = 1;

    printf("\nSistemas de Monitoramento");

    pthread_mutex_init(&lock, NULL);
    /*chama a função para inicializar o mutex com a herança de prioridade*/
    init_mutex_inh();
   
    /* inicializado com atributos padrão */
    pthread_attr_init (&tattr);

    /* obter o parâmetro de programação existente */
    pthread_attr_getschedparam (&tattr, &param);

    /* definir a prioridade; outros parâmetros não mudaram */
    param.sched_priority = 10;
    /* definindo o novo parâmetro de escalonamento */
    pthread_attr_setschedparam (&tattr, &param);
    pthread_create(&th_gas, &tattr, thread_duto_gas, NULL);

    /* definir a prioridade; outros parâmetros não mudaram */
    param.sched_priority = 5;
    /* definindo o novo parâmetro de escalonamento */
    pthread_attr_setschedparam (&tattr, &param);
    pthread_create(&th_oleo, &tattr, thread_duto_oleo, NULL);

    /* definir a prioridade; outros parâmetros não mudaram */
    param.sched_priority = 3;
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
   if ((rtn = pthread_mutexattr_init(&mutexattr_prioceiling)) != 0)
      fprintf(stderr,"pthread_mutexattr_init: %s",strerror(rtn)); //mensagem em caso de haver erro
   if ((rtn = pthread_mutexattr_getprotocol(&mutexattr_prioceiling, &mutex_protocol)) != 0)
      fprintf(stderr,"pthread_mutexattr_getprotocol: %s",strerror(rtn)); //mensagem em caso de haver erro
   
   /* pega a prioridade atual */
   high_prio = sched_get_priority_max(SCHED_FIFO);

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

   /* Define o protocolo do mutex como PROTECT - teto de prioridade*/  
   if ((rtn = pthread_mutexattr_setprotocol(&mutexattr_prioceiling, PTHREAD_PRIO_PROTECT)) != 0)
    fprintf(stderr,"pthread_mutexattr_setprotocol: %s",strerror(rtn)); //mensagem em caso de haver erro

   /* Define o o teto de prioridade inicial*/
   if ((rtn = pthread_mutexattr_setprioceiling(&mutexattr_prioceiling, high_prio)) != 0)
      fprintf(stderr,"pthread_mutexattr_setprotocol: %s",strerror(rtn)); //mensagem em caso de haver erro

   /* Inicialize mutex com o objeto de atributo */
   if ((rtn = pthread_mutex_init(&lock,&mutexattr_prioceiling)) != 0)
      fprintf(stderr,"pthread_mutexattr_init: %s",strerror(rtn)); //mensagem em caso de haver erro

#ifdef DEBUG 
   /*verificação para identificar se o protocolo foi realmente atribuído */
   if ((rtn = pthread_mutexattr_getprotocol(&mutexattr_prioceiling, &mutex_protocol)) != 0)
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