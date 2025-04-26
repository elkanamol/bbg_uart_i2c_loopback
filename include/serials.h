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
 * @brief Read data from the serial port
 *
 * @param fd File descriptor of the open serial port
 * @param data Buffer to store read data
 * @param length Maximum number of bytes to read
 * @return ssize_t Number of bytes read on success, negative error code on failure
 */
ssize_t serial_read(int fd, uint8_t *data, uint16_t length);

/**
 * @brief Write data to the serial port
 *
 * @param fd File descriptor of the open serial port
 * @param data Data to write
 * @param length Number of bytes to write
 * @return ssize_t number of writen bytes on success, negative -1 error code on failure
 */
ssize_t serial_write(int fd, uint8_t *data, uint16_t length);

/**
 * @brief Send a packet with CRC checksum over the serial port
 *
 * @param fd File descriptor of the open serial port
 * @param data Data buffer to send
 * @param length Length of the data buffer
 * @return ssize_t 0 on success, negative error code on failure
 */
ssize_t serial_send_packet_crc(int fd, uint8_t *data, uint16_t length);

/**
 * @brief Restart a serial port connection
 *
 * @param fd Current file descriptor of the serial port
 * @param port Path to the serial port device (e.g., "/dev/ttyS1")
 * @param baudrate Baud rate to set (use standard values like B9600, B115200)
 * @return int File descriptor of the restarted serial port on success, negative error code on failure
 */
int serial_restart(int fd, char *port, uint32_t baudrate);



#endif // SERIALS_H
