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

uint32_t x = 0;

void hello_task2(void *pvParameter){
    while(1){
        taskENTER_CRITICAL(&mutual_exclusion); // O taskENTER_CRITICAL está depreciado, pode-se usar o portENTER_CRITICAL
	    x++;
        taskEXIT_CRITICAL(&mutual_exclusion);// O taskEXIT_CRITICAL está depreciado, pode-se usar o portEXIT_CRITICAL
        printf("\nTask 2: %u", x);
	    vTaskDelay(1000 / portTICK_RATE_MS); 
	}
}
 

void hello_task1(void *pvParameter){
    while(1){
        taskENTER_CRITICAL(&mutual_exclusion); // O taskENTER_CRITICAL está depreciado, pode-se usar o portENTER_CRITICAL
	    x++;
        taskEXIT_CRITICAL(&mutual_exclusion); // O taskEXIT_CRITICAL está depreciado, pode-se usar o portEXIT_CRITICAL
        printf("\nTask 1: %u", x);
	    vTaskDelay(1000 / portTICK_RATE_MS);
	}
}

void app_main(){
    uint32_t x = 0;
    nvs_flash_init();

    taskENTER_CRITICAL(&mutual_exclusion); // O taskENTER_CRITICAL está depreciado, pode-se usar o portENTER_CRITICAL
    x++;
    taskEXIT_CRITICAL(&mutual_exclusion); // O taskEXIT_CRITICAL está depreciado, pode-se usar o portEXIT_CRITICAL

    xTaskCreatePinnedToCore(&hello_task1, "hello_task1", 4094, NULL, 3, NULL, 1);
    xTaskCreatePinnedToCore(&hello_task2, "hello_task2", 4094, NULL, 3, NULL, 1);
}