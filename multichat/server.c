#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8888
#define BUFFER_SIZE 1024

int client1_fd, client2_fd;

void *relay(void *arg) {
    int from_fd = *(int *)arg;
    int to_fd = (from_fd == client1_fd) ? client2_fd : client1_fd;
    char buffer[BUFFER_SIZE];
    int len;

    while ((len = recv(from_fd, buffer, sizeof(buffer), 0)) > 0) {
        send(to_fd, buffer, len, 0);
    }

    printf("One client disconnected. Closing both connections.\n");
    close(client1_fd);
    close(client2_fd);
    exit(0);  // Exit server when one client disconnects

    return NULL;
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Create server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 2) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d. Waiting for 2 clients...\n", PORT);

    // Accept client 1
    client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
    if (client_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    client1_fd = client_socket;
    printf("Client 1 connected.\n");

    // Accept client 2
    client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
    if (client_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    client2_fd = client_socket;
    printf("Client 2 connected.\n");

    // Create threads for both clients
    pthread_t t1, t2;
    pthread_create(&t1, NULL, relay, &client1_fd);
    pthread_create(&t2, NULL, relay, &client2_fd);

    // Wait for threads to finish (they won't unless a client disconnects)
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    close(server_fd);
    return 0;
}
