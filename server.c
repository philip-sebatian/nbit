#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define max_size 1024
char buffer[max_size];

void main(){
    int sock;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    bind(sock,(struct sockaddr *)&server,sizeof (server));
    printf("Listening in port %d\n", 8080);
    int ll = recvfrom(sock, buffer, sizeof(buffer),0,(struct sockaddr *)&client,&len);
    buffer[ll]='\0';
    printf("%s",buffer);
    char *message = "good morning";
    sendto(sock, message, strlen(message),0,(struct sockaddr *)&client, sizeof(client));
    close(sock);
}