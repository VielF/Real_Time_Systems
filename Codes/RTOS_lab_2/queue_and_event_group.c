#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <freertos/event_groups.h>
#include <esp_system.h>
#include <esp_log.h>
 
QueueHandle_t buffer;  //Cria um objeto da queue
//EventGroupHandle_t evt; //Cria um objeto do Grupo de Eventos

//#define EV_1SEG (1<<0)
// #define EV_1 (1<<0)
// #define EV_2 (1<<1)
// #define EV_3 (1<<2)

void task1(void *pvParameter){
    //EventBits_t x;
    uint32_t data_to_send = 0;
    while(1){
        // //Vamos esperar pelo evento (EV_1SEG) por no maximo 1000ms
        // x = xEventGroupWaitBits(evt, EV_1SEG, true, true, pdMS_TO_TICKS(1000));
        // x = xEventGroupWaitBits(evt, EV_1 | EV_2 | EV_3, true, true, pdMS_TO_TICKS(1000));
        // x = xEventGroupWaitBits(evt, EV_1 | EV_2 | EV_3, true, false, pdMS_TO_TICKS(1000));
        // //Se X & EV_1SEG (mascara binaria), significa que o evento ocorreu
        // if (x & EV_1SEG){
        //     printf("\nT1 OK");
        // }else{
        //     printf("\nT1 Event group TIMEOUT");
        // }

        if (data_to_send < 10){
            xQueueSend(buffer, &data_to_send, pdMS_TO_TICKS(0));
            data_to_send++;
        }else{
            vTaskDelay(2000/portTICK_PERIOD_MS);
            data_to_send = 0;
        }
        vTaskDelay(2000/portTICK_PERIOD_MS);
    }
}
 
void task2(void *pvParameter){
    uint32_t data_to_recive = 0;
    //uint8_t i;
    while(1){
        // //Envia 2 eventos antes do timeout
        // for ( i = 0; i < 2; i++){
        //     xEventGroupSetBits(evt, EV_1SEG);//Configura o BIT (EV_1SEG) em 1
        //     vTaskDelay(333/portTICK_PERIOD_MS);
        // }
        // vTaskDelay(1000/portTICK_PERIOD_MS);

        if (xQueueReceive(buffer, &data_to_recive, pdMS_TO_TICKS(1000)) == true){
            printf("\nItem recebido: %u", data_to_recive);
        }else{
            printf("\nItem nao recebido, timeout expirou!");
        }
    }
}
 
void app_main()
{
    buffer = xQueueCreate(10, sizeof(uint32_t));
    //evt = xEventGroupCreate();//Cria o grupo de eventos
    xTaskCreatePinnedToCore(&task1, "task1", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(&task2, "task2", 4096, NULL, 1, NULL, 0);
}