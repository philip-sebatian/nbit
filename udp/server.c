// udp_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);  // UDP socket
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server info
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP server listening on port %d...\n", PORT);

    while (1) {
        int len = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                           (struct sockaddr*)&client_addr, &addr_len);
        buffer[len] = '\0';
        printf("Client: %s\n", buffer);

        // Echo back
        sendto(sockfd, buffer, len, 0,
               (struct sockaddr*)&client_addr, addr_len);
    }

    close(sockfd);
    return 0;
}
