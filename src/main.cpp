#include <iostream>
#include "esp_timer.h" // ESP-IDF timer library
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <cstring>
#include "UART_Protocol.h"

void print(void *pvParameters)
{
    protocol_config config;
    config.port = UART_NUM_1;
    config.baudRate = 115200;

    UARTProtocol protocol(config, 0xAA, 100);
    protocol.begin();

    uint8_t recievedCommand = 0x00;

    uint8_t data[5] = {0x60, 0x61, 0x62, 0x63, 0x64};

    uint8_t readData[5];

    for (;;)
    {
        uint8_t command = 0x01;
        protocol.SendCommand(command);
        protocol.SendData(data, 5);
        protocol.ReadCommand(recievedCommand, 2000);


        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

extern "C" void app_main()
{
    xTaskCreate(print, "printHelloWorld", 4096, NULL, 5, NULL);
}