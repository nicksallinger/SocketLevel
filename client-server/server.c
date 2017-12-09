//
//  server.c
//  ECE1150
//
//  Created by Nicholas Sallinger on 11/20/17.
//  Copyright © 2017 Nicholas Sallinger. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>


char* reverse(char *str)
{
    static int i=0;
    static char rev[2048];
    
    if(*str)
    {
        reverse(str+1);
        rev[i++] = *str;
    }
    return rev;
}

int main(){
    int server_fd, new_socket, n, client_addr_size;
    char* reversed;
    char buffer[2048];
    struct sockaddr_in server_addr, clientAddr;
    
    if ((server_fd = socket(PF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket failure");
        exit(EXIT_FAILURE);
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(server_addr.sin_zero, '\0', sizeof server_addr.sin_zero);
    
    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr))<0)
    {
        perror("Binding failure");
    }
    
    if(listen(server_fd,0)==0)
        printf("Listening\n");
    else{
        perror("Listen failure");
        exit(EXIT_FAILURE);
    }
    
    client_addr_size = sizeof(clientAddr);
    
    if ((new_socket = accept(server_fd, (struct sockaddr *) &clientAddr, &client_addr_size))<0)
    {
        perror("Accept failure");
        exit(EXIT_FAILURE);
    }
    
    n = recv(new_socket, buffer, 1024, 0);
    if (n < 0){
        perror("ERROR receiving from socket");
    }

    printf("Server received: %s\n",buffer);

    reversed = reverse(buffer);
    
    printf("Server sent: %s\n",reversed);
    
    strcpy(buffer,reversed);
    n = write(new_socket,buffer,1024,0);
    if (n < 0){
        perror("ERROR writing to the socket");
    }
    
    return 0;
}
