/*
Arquivo: hello_word_and_blink_task.c
Autor: Felipe Viel
Função do arquivo: Cria uma task para printar o Hello World e uma para blink. Baseado no exempo do ESP-IDF
Criado em 17 de novembro de 2020
Modificado em 17 de novembro de 2020

*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "nvs_flash.h"
 
#define BLINK_GPIO 2
 
 
void hello_task(void *pvParameter){
 
	while(1)
	{
	    printf("Hello world!\n");
	    vTaskDelay(100 / portTICK_RATE_MS);
	}
}
 
void blinky(void *pvParameter){
 
    gpio_pad_select_gpio(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while(1) {
        /* Blink off (output low) */
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(1000 / portTICK_RATE_MS);
        /* Blink on (output high) */
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}
 
 
void app_main(){

    nvs_flash_init();
    xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);
    xTaskCreate(&blinky, "blinky", 1024,NULL,5,NULL );
}