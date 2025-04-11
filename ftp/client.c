#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    while (1) {
        printf("ftp> ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // remove newline

        send(sock, buffer, strlen(buffer), 0);

        if (strncmp(buffer, "UPLOAD ", 7) == 0) {
            FILE *fp = fopen(buffer + 7, "rb");
            if (!fp) {
                perror("File open error");
                continue;
            }

            int bytes;
            while ((bytes = fread(buffer, 1, BUFFER_SIZE, fp)) > 0) {
                send(sock, buffer, bytes, 0);
            }

            send(sock, "EOF", 3, 0);
            fclose(fp);
        }

        else if (strncmp(buffer, "DOWNLOAD ", 9) == 0) {
            FILE *fp = fopen(buffer + 9, "wb");
            if (!fp) {
                perror("File create error");
                continue;
            }

            int bytes;
            while ((bytes = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
                if (strncmp(buffer, "EOF", 3) == 0) break;
                fwrite(buffer, 1, bytes, fp);
                if (bytes < BUFFER_SIZE) break; // May signal end of transmission
            }

            fclose(fp);
        }

        else if (strncmp(buffer, "EXIT", 4) == 0) {
            break;
        }

        else {
            recv(sock, buffer, BUFFER_SIZE, 0);
            printf("Server: %s\n", buffer);
        }
    }

    close(sock);
    return 0;
}
