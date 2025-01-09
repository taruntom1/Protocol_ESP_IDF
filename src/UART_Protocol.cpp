#include "UART_Protocol.h"

UARTProtocol::UARTProtocol(protocol_config config)
    : config{config},
      header{config.header},
      maxPacketSize{config.maxPacketSize} {}

void UARTProtocol::begin()
{
    uart_config.baud_rate = config.baudRate;

    ESP_ERROR_CHECK(uart_param_config(config.port, &uart_config));

    ESP_ERROR_CHECK(uart_set_pin(config.port, config.pinTX, config.pinRX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    ESP_ERROR_CHECK(uart_driver_install(config.port, config.bufferSize, config.bufferSize, 0, nullptr, 0));

    ESP_LOGV("UART", "UART1 initialized successfully");
}

void UARTProtocol::SendCommand(const uint8_t &command)
{
    static uint8_t commandArr[2] = {header, 0x00};
    commandArr[1] = command;
    ESP_LOGV("UART", "Sending command: %d", command);
    uart_write_bytes(config.port, commandArr, 2);
}

void UARTProtocol::SendData(const uint8_t *data, const uint8_t length)
{
    uart_write_bytes(config.port, data, length);
    ESP_LOGV("UART", "Sent data of length: %d", length);
}

bool UARTProtocol::ReadCommand(uint8_t &command)
{

    while (uart_read_bytes(config.port, &command, 1, 0) > 0)
    {
        if (command == header)
        {
            ESP_LOGV("UART", "Header received");
            if (uart_read_bytes(config.port, &command, 1, pdMS_TO_TICKS(10)) > 0)
            {
                ESP_LOGV("UART", "Command read: %d", command);
                return true;
            }
            else
            {
                ESP_LOGI("UART", "Timeout reading command");
                return false;
            }
        }
    }
    ESP_LOGV("UART", "No header received");
    return false;
}

bool UARTProtocol::ReadCommand(uint8_t &command, uint32_t timeout_ms)
{
    static uint32_t tickCount;
    tickCount = xTaskGetTickCount();
    ESP_LOGV("UART", "Reading command with timeout");
    while (xTaskGetTickCount() - tickCount < pdMS_TO_TICKS(timeout_ms))
    {
        while (uart_read_bytes(config.port, &command, 1, 0) > 0)
        {
            if (command == header)
            {
                ESP_LOGV("UART", "Header received");
                if (uart_read_bytes(config.port, &command, 1, pdMS_TO_TICKS(10)) > 0)
                {
                    ESP_LOGV("UART", "Command read: %d", command);
                    return true;
                }
                else
                {
                    ESP_LOGI("UART", "Timeout reading command");
                    return false;
                }
            }
        }
    }
    ESP_LOGV("UART", "No header received within timeout");
    return false;
}

bool UARTProtocol::ReadData(uint8_t *data, uint8_t length)
{
    if (uart_read_bytes(config.port, data, length, pdMS_TO_TICKS(1000)) >= length)
    {
        ESP_LOGV("UART", "Received data of length: %d", length);
        return true;
    }
    else
    {
        ESP_LOGI("UART", "Timeout reading data");
        return false;
    }
}

bool UARTProtocol::ReadData(uint8_t *data, uint8_t length, uint32_t timeout_ms)
{
    if (uart_read_bytes(config.port, data, length, pdMS_TO_TICKS(timeout_ms)) >= length)
    {
        ESP_LOGV("UART", "Received data of length: %d", length);
        return true;
    }
    else
    {
        ESP_LOGI("UART", "Timeout reading data");
        return false;
    }
}
