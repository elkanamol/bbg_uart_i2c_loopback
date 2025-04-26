#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>

#include "../include/uart.h"    // Use relative path
#include "../include/serials.h" // Use relative path

int main(int argc, char *argv[])
{

    // Initialize UART
    char *port = "/dev/ttyS0";
    int uart_fd = serial_open(port, B115200);
    if (uart_fd < 0)
    {
        fprintf(stderr, "Failed to initialize UART (error code: %d)\n", uart_fd);
        return EXIT_FAILURE;
    }
    char txBuffer[MAX_UART_BUFFER_SIZE];
    /**
     * menu for user to select which function to run
     * 1. send data to UART
     * 2. receive data from UART
     * 3. exit
     */
    int choice;
    char prompt[] = "Please select an option:\n \
                    1. Send data to UART\n \
                    2. Send data with CRC to UART\n \
                    3. Receive data from UART\n \
                    4. Restart UART\n \
                    5. Exit\n";
    while (1)
    {
        printf("%s", prompt);
        if (scanf("%d", &choice) < 0)
        {
            printf("Error reading input\n");
            break;
        }
        switch (choice)
        {
        case 1:
            if (uart_fd < 0)
            {
                printf("UART is not initialized. Please restart it first.\n");
                break;
            }
            // send data to UART
            memset(txBuffer, 0, sizeof(txBuffer));
            printf("Enter data to send: ");
            if (scanf("%s", txBuffer) < 0)
            {
                printf("Error reading input\n");
                break;
            }
            ssize_t bytes_sent = serial_write(uart_fd, (uint8_t *)txBuffer, strlen(txBuffer));
            printf("Data sent [%d] bytes\n", bytes_sent);
            break;
        case 2:
            if (uart_fd < 0)
            {
                printf("UART is not initialized. Please restart it first.\n");
                break;
            }
            // send data with CRC to UART
            memset(txBuffer, 0, sizeof(txBuffer));
            printf("Enter data to send: ");
            if (scanf("%s", txBuffer) < 0)
            {
                printf("Error reading input\n");
                break;
            }
            ssize_t ret = serial_send_packet_crc(uart_fd, (uint8_t *)txBuffer, strlen(txBuffer));
            if (ret < 0)
            {
                printf("Error sending data\n");
                break;
            }
            printf("Data sent [%d] bytes\n", ret);
            break;

        case 3:
            // receive data from UART
            if (uart_fd < 0)
            {
                printf("UART is not initialized. Please restart it first.\n");
                break;
            }
            printf("Receiving data from UART...\n");
            char rxBuffer[MAX_UART_BUFFER_SIZE] = {0};
            ssize_t bytesRead = serial_read(uart_fd, (uint8_t *)rxBuffer, sizeof(rxBuffer));
            if (bytesRead > 0)
            {
                printf("Received data: %s\n", rxBuffer);
            }
            break;
        case 4:
            // restart UART
            int new_fd = serial_restart(uart_fd, port, B115200);
            if (new_fd < 0)
            {
                printf("Failed to restart UART. Error code: %d\n", new_fd);
                uart_fd = -1;
            }
            else
            {
                uart_fd = new_fd;
                printf("UART restarted successfully\n");
            }
            break;
        case 5:
            // exit
            close(uart_fd);
            printf("Exiting...\n");
            return 0;
        default:
            printf("Invalid choice\n");
            break;
        }
    }

    // Clean up
    close(uart_fd);
    return 0;
}
