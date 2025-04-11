#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (strncmp(buffer, "UPLOAD ", 7) == 0) {
            char *filename = buffer + 7;
            FILE *fp = fopen(filename, "wb");
            if (!fp) {
                perror("File open failed");
                continue;
            }

            int bytes;
            while ((bytes = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
                if (strcmp(buffer, "EOF") == 0) break;
                fwrite(buffer, 1, bytes, fp);
            }

            fclose(fp);
            printf("Uploaded file: %s\n", filename);
        }

        else if (strncmp(buffer, "DOWNLOAD ", 9) == 0) {
            char *filename = buffer + 9;
            FILE *fp = fopen(filename, "rb");
            if (!fp) {
                char *msg = "File not found";
                send(client_socket, msg, strlen(msg), 0);
                continue;
            }

            int bytes;
            while ((bytes = fread(buffer, 1, BUFFER_SIZE, fp)) > 0) {
                send(client_socket, buffer, bytes, 0);
            }

            send(client_socket, "EOF", 3, 0);
            fclose(fp);
            printf("Sent file: %s\n", filename);
        }

        else if (strncmp(buffer, "EXIT", 4) == 0) {
            printf("Client disconnected.\n");
            break;
        }

        else {
            char *msg = "Unknown command";
            send(client_socket, msg, strlen(msg), 0);
        }
    }

    close(client_socket);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("FTP server started on port %d...\n", PORT);
    while (1) {
        client_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        printf("Client connected.\n");
        handle_client(client_socket);
    }

    return 0;
}
