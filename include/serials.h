#ifndef SERIALS_H
#define SERIALS_H

#include <stdint.h>
#include <termios.h> // For baudrate constants

/**
 * @brief Maximum buffer size for UART communication
 */
#define MAX_UART_BUFFER_SIZE 256

/**
 * @brief Initialize and open a serial port
 *
 * @param port Path to the serial port device (e.g., "/dev/ttyS1")
 * @param baudrate Baud rate to set (use standard values like B9600, B115200)
 * @return int File descriptor on success, negative error code on failure
 */
int serial_open(const char *port, uint32_t baudrate);

/**
 * @brief Read a single byte from the serial port
 *
 * @param fd File descriptor of the open serial port
 * @return uint8_t The byte read from the serial port
 */
uint8_t serial_read(int fd);

/**
 * @brief Write data to the serial port
 *
 * @param fd File descriptor of the open serial port
 * @param data Data to write
 * @param length Number of bytes to write
 * @return uint8_t 0 on success, negative error code on failure
 */
uint8_t serial_write(int fd, uint8_t *data, uint16_t length);

/**
 * @brief Send a packet with CRC checksum over the serial port
 *
 * @param fd File descriptor of the open serial port
 * @param data Data buffer to send
 * @param length Length of the data buffer
 * @return uint8_t 0 on success, negative error code on failure
 */
uint8_t serial_send_packet_crc(int fd, uint8_t *data, uint16_t length);

#endif // SERIALS_H
