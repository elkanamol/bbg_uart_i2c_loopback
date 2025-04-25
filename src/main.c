#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>

#include "../include/uart.h"    // Use relative path
#include "../include/serials.h" // Use relative path

// Rest of the file remains the same

int main(int argc, char *argv[])
{

        // Initialize UART
    int uart_fd = serial_open("/dev/ttyS1", B115200);
    if (uart_fd < 0)
    {
        fprintf(stderr, "Failed to initialize UART (error code: %d)\n", uart_fd);
        return EXIT_FAILURE;
    }
    char txBuffer[256];
    /**
     * menu for user to select which function to run
     * 1. send data to UART
     * 2. receive data from UART
     * 3. exit
     */
    int choice;
    char prompt[] = "Please select an option:\n \
                    1. Send data to UART\n      \
                    2. Receive data from UART\n \
                    3. Exit\n";
    while (1)
    {
        printf("%s", prompt);
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            // send data to UART
            printf("Enter data to send: ");
            scanf("%s", txBuffer);
            serial_write(uart_fd, txBuffer, strlen(txBuffer));
            printf("Data sent\n");
            break;
        case 2:
            // receive data from UART
            printf("Receiving data from UART...\n");
            char rxBuffer[256];
            ssize_t bytesRead = read(uart_fd, rxBuffer, sizeof(rxBuffer));
            if (bytesRead > 0)
            {
                printf("Received data: %s\n", rxBuffer);
            }
            break;
        case 3:
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
