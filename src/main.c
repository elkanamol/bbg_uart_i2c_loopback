#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>

#include "../include/uart.h"    // Use relative path
#include "../include/serials.h" // Use relative path

#define BAUD_RATE B115200
#define DEFAULT_PORT "/dev/ttyS1"

int main(int argc, char *argv[])
{

    // Initialize UART
    int uart_fd = serial_open(DEFAULT_PORT, BAUD_RATE);
    if (uart_fd < 0)
    {
        fprintf(stderr, "Failed to initialize UART (error code: %d)\n", uart_fd);
        return EXIT_FAILURE;
    }
    int choice;
    char prompt[] = "Please select an option:\n \
    1. Send data to UART\n \
    2. Send data with CRC to UART\n \
    3. Send 0-9 characters\n \
    4. Receive data from UART\n \
    5. Restart UART\n \
    6. Exit\n";
    while (1)
    {
        char txBuffer[MAX_UART_BUFFER_SIZE];
        printf("%s", prompt);
        if (scanf("%d", &choice) < 0)
        {
            printf("Error reading input\n");
            break;
        }
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        if (uart_fd < 0)
        {
            printf("UART is not initialized. Please restart it first.\n");
            continue;
        }
        switch (choice)
        {
        case 1: // send data to UART
            printf("Enter data to send: ");
            if (scanf("%255s", txBuffer) < 0)
            {
                printf("Error reading input\n");
                break;
            }
            int bytes_sent = serial_write(uart_fd, (uint8_t *)txBuffer, strlen(txBuffer));
            if (bytes_sent < 0)
            {
                printf("Error sending data\n");
            }
            else
            {
                printf("Data sent [%d] bytes\n", bytes_sent);
            }
            break;
        case 2: // send data with CRC to UART
            printf("Enter data to send: ");
            if (scanf("%255s", txBuffer) < 0)
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
            // send data to UART
            sprintf(txBuffer, "0123456789");
            bytes_sent = serial_write(uart_fd, (uint8_t *)txBuffer, strlen(txBuffer));
            printf("Data sent [%d] bytes\n", bytes_sent);
            break;

        case 4:
            // receive data from UART
            printf("Receiving data from UART...\n");
            char rxBuffer[MAX_UART_BUFFER_SIZE] = {0};
            ssize_t bytesRead = serial_read(uart_fd, (uint8_t *)rxBuffer, sizeof(rxBuffer));
            if (bytesRead > 0)
            {
                rxBuffer[sizeof(rxBuffer) - 1] = '\0';
                printf("Received data: %s\n", rxBuffer);
            }
            break;
        case 5:
            // restart UART
            {
                int new_fd = serial_restart(uart_fd, DEFAULT_PORT, BAUD_RATE);
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
            }
            break;

        case 6:
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
