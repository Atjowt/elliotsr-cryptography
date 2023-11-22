#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sodium.h>

#include "server.h"

char RAM[MAX_FILE_SIZE][MAX_FILES];

int main(void) {

    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        fprintf(stderr, "Error creating server socket\n");
        return 1;
    }

    // Initialize server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        fprintf(stderr, "Error binding server socket\n");
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        fprintf(stderr, "Error listening on server socket\n");
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept a connection from a client
    if ((client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len)) == -1) {
        fprintf(stderr, "Error accepting connection\n");
        return 1;
    }

    printf("Client connected\n");

    // Receive data from the client
    while (1) {

        ssize_t bytes;
        unsigned op;
        unsigned id;
        size_t size;

        // Recieve request type
        bytes = recv(client_socket, &op, sizeof(op), 0); // receive operation type
        if (bytes <= 0) {
            printf("Received termination signal\n");
            break;
        }

        switch (op) {

            case READ: {
                printf("Received read request");
                bytes = recv(client_socket, &id, sizeof(id), 0); // receive file id
                printf(" (id: %u)", id);
                bytes = recv(client_socket, &size, sizeof(size), 0); // receive read size
                printf(" (size: %zu)", size);
                printf("\n");
                bytes = send(client_socket, RAM[id], size, 0); // send file data
                printf("Sent file data\n");
                break;
            }

            case WRITE: {
                printf("Received write request");
                bytes = recv(client_socket, &id, sizeof(id), 0); // receive file id
                printf(" (id: %u)", id);
                bytes = recv(client_socket, &size, sizeof(size), 0); // receive write size
                printf(" (size: %zu)", size);
                printf("\n");
                bytes = recv(client_socket, RAM[id], sizeof(RAM[id]), 0); // receive data
                printf("Received data\n");

                // ---- DEBUG
                printf("'");
                fwrite(RAM[id], 1, size, stdout);
                printf("'\n");
                // ---- DEBUG

                break;
            }
        }

    }

    printf("Closing server...\n");

    // Close the sockets
    close(client_socket);
    close(server_socket);

    return 0;
}
