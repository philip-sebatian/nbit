#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define max_size 1024
char buffer[max_size];
int mp[max_size];
char clean_buff[max_size];
void main(){
    int server, client;
    struct sockaddr_in server_s, client_s;
    socklen_t cl = sizeof(client_s);
    server  = socket(AF_INET, SOCK_STREAM, 0);

    server_s.sin_family = AF_INET;
    server_s.sin_port = htons(8080);
    server_s.sin_addr.s_addr = INADDR_ANY;
    
    bind(server, (struct sockaddr *)&server_s, sizeof(server_s));
    listen(server,3);
    client = accept(server, (struct sockaddr *)&client_s, &cl);
    int len;
    memset(mp,0,sizeof(mp));
    memset(clean_buff,'\0', sizeof(clean_buff));
    while((len = recv(client, buffer, sizeof(buffer),0))>0){
        buffer[len]='\0';
        printf("%s", buffer);
        for(int i =0 ; i <  strlen(buffer);i++){
            mp[(int)buffer[i]]++;
        }
        int ptr =0;
        for(int i =0; i < strlen(buffer);i++){
            if(mp[(int)buffer[i]]==1){
                clean_buff[ptr]=buffer[i];
                ptr++;
            }
        }
        printf("%s\n",clean_buff);
        send(client, clean_buff, strlen(clean_buff),0);
        memset(mp,0,sizeof(mp));
        memset(clean_buff,'\0', sizeof(clean_buff));
    }
    
    close(server);
    close(client);

}