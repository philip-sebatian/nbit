#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

char buffer [1024];

void main(){
    int sock;
    struct sockaddr_in server;
    char * message = "hello";
    sock = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family= AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server, sizeof(server));
    printf("%s", buffer);
    while(fgets(buffer, sizeof(buffer), stdin)){
        send(sock,buffer, strlen(buffer),0);
        memset(buffer, sizeof(buffer),'\0');
        recv(sock, buffer, sizeof(buffer),0);
        printf("%s\n",buffer);
    }
}