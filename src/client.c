#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sodium.h>

#include "client.h"

#define BUFFER_SIZE 256

int main(void) {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        fprintf(stderr, "Error creating client socket\n");
        return 1;
    }

    // Initialize server address struct
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        fprintf(stderr, "Error connecting client\n");
        return 1;
    }

    // Send data to the server
    while (1) {

        printf("What to do? (r/w/q) ");
        fgets(buffer, sizeof(buffer), stdin); // get user input

        unsigned op;
        unsigned id;
        size_t size;
        ssize_t bytes;
        
        if (strcmp(buffer, "r\n") == 0) {

            op = READ;

            printf("Enter file id: ");
            fscanf(stdin, "%u", &id);
            fgetc(stdin);

            printf("Enter number of bytes to read: ");
            fscanf(stdin, "%zu", &size);
            fgetc(stdin);

            bytes = send(client_socket, &op, sizeof(op), 0); // send read operation signal
            bytes = send(client_socket, &id, sizeof(id), 0); // send file id
            bytes = send(client_socket, &size, sizeof(size), 0); // send read size

            printf("Sent read request\n");

            bytes = recv(client_socket, buffer, size, 0); // receive data

            printf("Received data\n");

            // ---- DEBUG
            printf("'");
            fwrite(buffer, 1, size, stdout);
            printf("'\n");
            // ---- DEBUG

        } else if (strcmp(buffer, "w\n") == 0) {

            op = WRITE;

            printf("Enter file id: ");
            fscanf(stdin, "%u", &id);
            fgetc(stdin);

            printf("Enter data to write: ");
            fgets(buffer, BUFFER_SIZE, stdin);

            size = strlen(buffer);

            bytes = send(client_socket, &op, sizeof(op), 0); // send write operation signal
            bytes = send(client_socket, &id, sizeof(id), 0); // send file id
            bytes = send(client_socket, &size, sizeof(size), 0); // send write size
            bytes = send(client_socket, buffer, size, 0); // send write data

            printf("Sent write request\n");

            // TODO: receive response from server ...

        } else if (strcmp(buffer, "q\n") == 0) {
            break; // Terminate client
        } else {
            fprintf(stderr, "Unknown operation '%s'\n", buffer);
        }

    }

    printf("Closing client...\n");

    // Close the socket
    close(client_socket);

    return 0;
}
