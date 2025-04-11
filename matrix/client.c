#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define max_size 1024

void main(){
    int server;
    struct sockaddr_in ss;


    int result[3][3];
    int matrix1 [3][3]={{1,2,3},{4,5,6},{7,8,9}};
    int matrix2 [3][3]={{1,2,3},{4,5,6},{7,8,9}};
    server = socket(AF_INET, SOCK_STREAM, 0);

    ss.sin_family = AF_INET;
    ss.sin_port = htons(8080);
    ss.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(server, (struct sockaddr *)&ss,sizeof(ss));

    send(server, matrix1, sizeof(matrix1), 0);
    send(server, matrix2, sizeof(matrix2), 0);
    recv(server, result, sizeof(result),0);

    for(int i =0 ; i < 3;i++){
        for(int j =0 ; j < 3;j++){
            printf("%d\t",result[i][j]);
        }
        printf("\n");
    }

    close(server);
}