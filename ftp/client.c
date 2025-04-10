#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void upload(int sock, char *filename) {
    char buffer[BUFFER_SIZE];
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("File not found.\n");
        return;
    }

    sprintf(buffer, "UPLOAD %s", filename);
    write(sock, buffer, strlen(buffer));

    read(sock, buffer, BUFFER_SIZE);  // wait for READY
    if (strncmp(buffer, "READY", 5) != 0) {
        printf("Server failed to prepare for upload.\n");
        return;
    }

    int len;
    while ((len = fread(buffer, 1, BUFFER_SIZE, fp)) > 0) {
        write(sock, buffer, len);
    }
    fclose(fp);
    write(sock, "EOF", 3);
    printf("Upload complete.\n");
}

void download(int sock, char *filename) {
    char buffer[BUFFER_SIZE];
    sprintf(buffer, "DOWNLOAD %s", filename);
    write(sock, buffer, strlen(buffer));

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Failed to open file for writing.\n");
        return;
    }

    int len;
    while ((len = read(sock, buffer, BUFFER_SIZE)) > 0) {
        if (strncmp(buffer, "ERROR", 5) == 0) {
            printf("File not found on server.\n");
            fclose(fp);
            remove(filename);
            return;
        }
        if (strncmp(buffer, "EOF", 3) == 0) break;
        fwrite(buffer, 1, len, fp);
    }
    fclose(fp);
    printf("Download complete.\n");
}

void list_files(int sock) {
    char buffer[BUFFER_SIZE];
    write(sock, "LIST", 4);

    while (1) {
        int len = read(sock, buffer, BUFFER_SIZE - 1);
        buffer[len] = '\0';
        if (strncmp(buffer, "EOF", 3) == 0) break;
        printf("%s", buffer);
    }
}

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char command[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("Connected to FTP server.\n");

    while (1) {
        printf("ftp> ");
        fgets(command, BUFFER_SIZE, stdin);
        command[strcspn(command, "\n")] = 0;

        if (strncmp(command, "upload ", 7) == 0) {
            upload(sock, command + 7);
        } else if (strncmp(command, "download ", 9) == 0) {
            download(sock, command + 9);
        } else if (strncmp(command, "list", 4) == 0) {
            list_files(sock);
        } else if (strncmp(command, "exit", 4) == 0) {
            write(sock, "EXIT", 4);
            break;
        } else {
            printf("Invalid command. Try: upload, download, list, exit\n");
        }
    }

    close(sock);
    return 0;
}
