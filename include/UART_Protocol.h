/**
 * @file UARTProtocol.h
 * @brief Header file for the UARTProtocol class, providing UART communication utilities.
 */

#ifndef UARTPROTOCOL_H
#define UARTPROTOCOL_H

#include <stdint.h>
#include "driver/uart.h"
#include "esp_log.h"

/**
 * @struct protocol_config
 * @brief Configuration structure for UARTProtocol.
 *
 * This structure holds the configuration parameters required to initialize
 * the UARTProtocol class, such as baud rate, UART port, buffer size, and pin settings.
 */
struct protocol_config
{
    int baudRate = 115200;         /**< Baud rate for UART communication. */
    uart_port_t port = UART_NUM_1; /**< UART port number. */
    uint16_t bufferSize = 1024;    /**< Size of the UART buffer. */
    uint8_t pinRX = 18;            /**< RX pin number. */
    uint8_t pinTX = 17;            /**< TX pin number. */
    uint8_t header = 0xAA;         /**< Header byte for the protocol. */
    uint8_t maxPacketSize = 100;   /**< Maximum size of a data packet. */
};

/**
 * @class UARTProtocol
 * @brief Provides a protocol layer for UART communication.
 *
 * The UARTProtocol class encapsulates UART communication functionality, including
 * sending and receiving commands and data, with header and checksum validation.
 */
class UARTProtocol
{
private:
    protocol_config config; /**< Configuration settings for the protocol. */
    uint8_t header;         /**< Header byte for the protocol. */
    uint8_t maxPacketSize;  /**< Maximum size of a data packet. */

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
    /**
     * @brief Constructs a UARTProtocol object.
     * @param config Configuration settings for the protocol.
     */
    UARTProtocol(protocol_config config);

    /**
     * @brief Initializes the UART protocol with the specified configuration.
     */
    void begin();

    /**
     * @brief Sends a command over UART.
     * @param commandType The type of command to send.
     */
    void SendCommand(const uint8_t &commandType);

    /**
     * @brief Sends data over UART.
     * @param data Pointer to the data array to send.
     * @param length Length of the data array.
     */
    void SendData(const uint8_t *data, const uint8_t length);

    /**
     * @brief Reads a command from UART.
     * @param commandType Reference to store the received command type.
     * @return True if a command was successfully read, false otherwise.
     */
    bool ReadCommand(uint8_t &commandType);

    /**
     * @brief Reads a command from UART with a timeout.
     * @param commandType Reference to store the received command type.
     * @param timeout Timeout duration in milliseconds.
     * @return True if a command was successfully read, false otherwise.
     */
    bool ReadCommand(uint8_t &commandType, uint32_t timeout);

    /**
     * @brief Reads data from UART with a timeout.
     * @param data Pointer to store the received data.
     * @param length Length of the data to read.
     * @param timeout Timeout duration in milliseconds.
     * @return True if data was successfully read, false otherwise.
     */
    bool ReadData(uint8_t *data, uint8_t length, uint32_t timeout);

    /**
     * @brief Reads data from UART.
     * @param data Pointer to store the received data.
     * @param length Length of the data to read.
     * @return True if data was successfully read, false otherwise.
     */
    bool ReadData(uint8_t *data, uint8_t length);
};

#endif // UART_PROTOCOL_H
