/*
Arquivo: task_mutual_exclusion.c
Autor: Felipe Viel
Função do arquivo: Cria tasks para printar e utilização exclusão mútua. Baseado no exempo do ESP-IDF
Criado em 17 de novembro de 2020
Modificado em 17 de novembro de 2020

*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
//#include "freertos/semphr.h"


static portMUX_TYPE mutual_exclusion = portMUX_INITIALIZER_UNLOCKED;

uint32_t secao_critica = 0;

void hello_task2(void *pvParameter){
    while(1){
        vTaskSuspendAll(); 
	    secao_critica+=10;
        if( !xTaskResumeAll()){
            taskYIELD ();
        }
        printf("\nTask 2: %u", secao_critica);
	    vTaskDelay(1000 / portTICK_RATE_MS); 
	}
}
 

void hello_task1(void *pvParameter){
    while(1){
        vTaskSuspendAll(); 
	    secao_critica++;
        if( !xTaskResumeAll()){
          taskYIELD ();
        }
        printf("\nTask 1: %u", secao_critica);
	    vTaskDelay(1000 / portTICK_RATE_MS);
	}
}

void app_main(){
    nvs_flash_init();

    xTaskCreatePinnedToCore(&hello_task1, "hello_task1", 4094, NULL, 3, NULL, 1);
    xTaskCreatePinnedToCore(&hello_task2, "hello_task2", 4094, NULL, 3, NULL, 1);

    vTaskSuspendAll(); 
	secao_critica++;
    if( !xTaskResumeAll()){
        taskYIELD ();
    }
    printf("\nTask IDLE: %u", secao_critica);

}