#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define max_size 1024

void main(){
    int server, client;
    struct sockaddr_in ss, cs;
    socklen_t ll = sizeof(cs);

    int matrix1[3][3],matrix2[3][3],result[3][3];
    server = socket(AF_INET, SOCK_STREAM, 0);
    ss.sin_family = AF_INET;
    ss.sin_port = htons(8080);
    ss.sin_addr.s_addr = INADDR_ANY;

    bind(server , (struct sockaddr *)&ss, sizeof(ss));
    listen(server, 2);

    client = accept(server, (struct sockaddr *)&cs,&ll);
    recv(client, matrix1,sizeof(matrix1),0);
    recv(client, matrix2,sizeof(matrix2),0);
    for(int i =0 ; i< 3;i++){
        for(int j =0 ; j<3 ; j++){
            result[i][j]=matrix1[i][j]+matrix2[i][j];
        }
    }
    send(client, result, sizeof(result), 0);
    close(server);
    close(client);
}