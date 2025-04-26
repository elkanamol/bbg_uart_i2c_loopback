#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "serials.h"
#include "crc.h"
#include "uart.h"


int serial_open(const char *port, uint32_t baudrate)
{
    int uart_fd = init_uart(port, baudrate);
    if (uart_fd < 0)
    {
        fprintf(stderr, "Failed to initialize UART (error code: %d)\n", uart_fd);
        return EXIT_FAILURE;
    }
    return uart_fd;
}

int serial_restart(int fd, char *port, uint32_t baudrate)
{
    close(fd);
    int uart_fd = serial_open(port, baudrate);
    if (uart_fd < 0)
    {
        perror("Failed to initialize UART at port");
        return uart_fd; // Return the actual error code, not EXIT_FAILURE
    }
    return uart_fd;
}
ssize_t serial_read(int fd, uint8_t *data, uint16_t length)
{
    ssize_t retval = read(fd, data, length);
    return retval;
}

ssize_t serial_write(int fd, uint8_t *data, uint16_t length)
{
    if (length > MAX_UART_BUFFER_SIZE || length == 0)
    {
        return -1;
    }

    ssize_t retval = write(fd, data, length);
    return retval;
}

ssize_t serial_send_packet_crc(int fd, uint8_t *data, uint16_t length)
{
    uint8_t data_with_crc[MAX_UART_BUFFER_SIZE] = {0};
    uint16_t crc = crc16(data, length);
    printf("CRC: %x\n", crc);
    crc16_update_buffer(crc, data_with_crc, length);
    ssize_t retval = serial_write(fd, data_with_crc, length + 2);
    printf("Data with CRC: %s\n", data_with_crc);
    return retval;
}



