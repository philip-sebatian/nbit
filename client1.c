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
    socklen_t len = sizeof(server);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    fgets(buffer,sizeof(buffer),stdin);
    sendto( sock, buffer, strlen(buffer),0,(struct sockaddr *)&server, sizeof(server));
    int ll = recvfrom(sock, buffer,sizeof(buffer),0,(struct sockaddr *)&server,&len);
    buffer[ll]='\0';
    printf("%s\n",buffer);
}