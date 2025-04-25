#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "serials.h"
#include "crc.h"
#include "uart.h"


int serial_open(const char *port, uint32_t baudrate)
{
    int uart_fd = init_uart("/dev/ttyS1", B115200);
    if (uart_fd < 0)
    {
        fprintf(stderr, "Failed to initialize UART (error code: %d)\n", uart_fd);
        return EXIT_FAILURE;
    }
    return uart_fd;
}
uint8_t serial_read(int fd)
{
    uint8_t data;
    read(fd, &data, sizeof(data));
    return data;
}

uint8_t serial_write(int fd, uint8_t data, uint16_t length)
{
    if (length > MAX_UART_BUFFER_SIZE || length == 0)
    {
        return -1;
    }

    ssize_t retval = write(fd, &data, length);
    return retval;
}

uint8_t serial_send_packet_crc(int fd, uint8_t *data, uint16_t length)
{
    uint8_t data_with_crc[length + 2];
    uint16_t crc = crc16(data, length);
    crc16_update_buffer(crc, data_with_crc, length);
    ssize_t retval = serial_write(fd, data_with_crc, length + 2);
    if (retval < 0)
    {
        return -1;
    }
    return 0;

}



