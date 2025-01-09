#ifndef UARTPROTOCOL_H
#define UARTPROTOCOL_H

#include <stdint.h>
#include "driver/uart.h"
#include "esp_log.h"

struct protocol_config
{
    int baudRate = 115200;
    uart_port_t port = UART_NUM_1;
    uint16_t bufferSize = 1024;
    uint8_t pinRX = 18;
    uint8_t pinTX = 17;
    uint8_t header = 0xAA;
    uint8_t maxPacketSize = 100;
};

class UARTProtocol
{
private:
    protocol_config config;
    uint8_t header;
    uint8_t maxPacketSize;

    uint8_t calculateChecksum(uint8_t commandType, uint8_t *data, uint8_t length); // length = length of data

    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
        .source_clk = UART_SCLK_APB, // Ensure all fields are initialized
    };

public:
    UARTProtocol(protocol_config config);

    void begin();

    void SendCommand(const uint8_t &commandType);
    void SendData(const uint8_t *data, const uint8_t length);
    bool ReadCommand(uint8_t &commandType);
    bool ReadCommand(uint8_t &commandType, uint32_t timeout);
    bool ReadData(uint8_t *data, uint8_t length, uint32_t timeout);
    bool ReadData(uint8_t *data, uint8_t length);
};

#endif // UARTPROTOCOL_H

// bool sendPacket(uint8_t commandType, uint8_t *parameters, uint8_t parameterCount, bool checkChecksum = true);

// void SendChecksum(uint8_t commandType, uint8_t *data, uint8_t length);
//

// bool VerifyChecksum(uint8_t &commandType, uint8_t *data, uint8_t length);
// bool receivePacket(uint8_t &commandType, uint8_t *parameters, uint8_t parameterCount, bool checkChecksum = true);

// bool available();