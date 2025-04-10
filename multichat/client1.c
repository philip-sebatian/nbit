#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8888
#define BUFFER_SIZE 1024

int sock;

void *receive_messages(void *arg) {
    char buffer[BUFFER_SIZE];
    int len;

    while ((len = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[len] = '\0';  // Null-terminate received string
        printf("Friend: %s", buffer);
    }

    printf("Disconnected from server.\n");
    exit(0);
    return NULL;
}

int main() {
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server info
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Localhost

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server. You can now chat:\n");

    // Create thread to receive messages
    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_messages, NULL);

    // Main thread sends messages
    while (fgets(buffer, BUFFER_SIZE, stdin)) {
        send(sock, buffer, strlen(buffer), 0);
    }

    close(sock);
    return 0;
}
