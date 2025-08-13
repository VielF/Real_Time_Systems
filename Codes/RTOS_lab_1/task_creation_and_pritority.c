/*
Arquivo: task_creation_and_priority.c
Autor: Felipe Viel
Função do arquivo: Cria duas tasks para printar o Hello World e exemplifica a função para direcionar para um core. Baseado no exempo do ESP-IDF
Criado em 17 de novembro de 2020
Modificado em 17 de novembro de 2020

*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_task.h"


void hello_task2(void *pvParameter){
    while(1){
	    printf("Hello world from Task 2!\n");
	    vTaskDelay(100 / portTICK_RATE_MS);
	}
}
 

void hello_task1(void *pvParameter){
    while(1){
	    printf("Hello world from Task 1!\n");
	    vTaskDelay(80 / portTICK_RATE_MS);
	}
}

void app_main(){

    nvs_flash_init();
    printf("\nMAIN priority: %d\n", ESP_TASK_MAIN_PRIO);

    //xTaskCreatePinnedToCore(&hello_task1, "hello_task1", 2048, NULL, 1, NULL, 0);
    xTaskCreate(&hello_task1, "hello_task1", 2048, NULL, 1, NULL);
    //xTaskCreatePinnedToCore(&hello_task1, "hello_task1", 2048, NULL, 1, NULL, 0);
    xTaskCreate(&hello_task2, "hello_task2", 2048, NULL, 20, NULL);
    /*
    Go to ESP_PATH\esp-idf\components\esp32\ and open cpu_start.c
    See the app_main() creation 
    */
}