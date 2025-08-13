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
#include "freertos/semphr.h"


SemaphoreHandle_t mutual_exclusion_mutex;

void print_name(const char *task_name){
    xSemaphoreTake(mutual_exclusion_mutex, portMAX_DELAY);
    vTaskSuspendAll ();
        printf((char *) task_name);
    if( !xTaskResumeAll()){
          taskYIELD ();
     }
    xSemaphoreGive(mutual_exclusion_mutex);
} 

void hello_task2(void *pvParameter){
    const char *my_name = "\nI am Task 2";
    while(1){
        print_name(my_name);
	    vTaskDelay(1000 / portTICK_RATE_MS);
	}
}
 

void hello_task1(void *pvParameter){
    const char *my_name = "\nI am Task 1";
    while(1){
        print_name(my_name);
	    vTaskDelay(1000 / portTICK_RATE_MS);
	}
}

void app_main(){
    const char *my_name = "\nI am IDLE function";
    nvs_flash_init();

    mutual_exclusion_mutex = xSemaphoreCreateMutex();
    //mutual_exclusion_mutex = xSemaphoreCreateCounting(3,3); //Couting Semaphore with 3 resourcers (critical sections) being managed

    if( mutual_exclusion_mutex != NULL ){
       // The semaphore was created successfully.
       // The semaphore can now be used.
       printf("Mutex was created\n");
    }

    xTaskCreatePinnedToCore(&hello_task1, "hello_task1", 2048, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(&hello_task2, "hello_task2", 2048, NULL, 1, NULL, 1);

    while(1){
        print_name(my_name);
	    vTaskDelay(1000 / portTICK_RATE_MS);
	}
}