#include "gps_uart.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

#define UART_TX_PIN 0
#define UART_RX_PIN 1

GPSUART::GPSUART(uart_inst_t *uart, uint baudrate) : _uart(uart) {
    initUART(baudrate);
}

void GPSUART::initUART(uint baudrate) {
    uart_init(_uart, baudrate);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_format(_uart, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(_uart, false);
}

bool GPSUART::readNMEASentence(char *buffer, size_t len) {
    size_t i = 0;
    bool start = false;

    while (i < len - 1) {
        if (uart_is_readable(_uart)) {
            char ch = uart_getc(_uart);

            if (ch == '$') {
                i = 0;        // Reset buffer
                start = true; // Start of new sentence
            }

            if (start) {
                buffer[i++] = ch;
                if (ch == '\n') {
                    buffer[i] = '\0';
                    return true;  // Complete sentence received
                }
            }
        }
    }

    buffer[i] = '\0';
    return false;
}
