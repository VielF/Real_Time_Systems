/*
Arquivo: hello_word_task.c
Autor: Felipe Viel
Função do arquivo: Cria uma task para printar o Hello World. Baseado no exempo do ESP-IDF
Criado em 17 de novembro de 2020
Modificado em 17 de novembro de 2020

*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
 
void hello_task(void *pvParameter)
{
    printf("Hello world!\n");
    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}
 
void app_main()
{
    nvs_flash_init();
    xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);
}