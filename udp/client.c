// udp_client.c
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
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);  // UDP socket
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server info
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1) {
        printf("You: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        sendto(sockfd, buffer, strlen(buffer), 0,
               (struct sockaddr*)&server_addr, addr_len);

        int len = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                           (struct sockaddr*)&server_addr, &addr_len);
        buffer[len] = '\0';
        printf("Server echoed: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
