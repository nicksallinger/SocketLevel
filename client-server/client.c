//
//  client.c
//  ECE1150
//
//  Created by Nicholas Sallinger on 11/20/17.
//  Copyright © 2017 Nicholas Sallinger. All rights reserved.
//

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>


int main(){
    int client_fd, n, addr_size;
    char buffer[2048];
    struct sockaddr_in server_addr;
    
    if ((client_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Socket error");
        return -1;
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(server_addr.sin_zero, '\0', sizeof server_addr.sin_zero);

    addr_size = sizeof server_addr;
   
    if (connect(client_fd, (struct sockaddr *) &server_addr, addr_size) < 0)
    {
        printf("Connect failure");
        return -1;
    }

    printf("Hello. Give me something to send to the server:\n");
    fgets(buffer,2048,stdin);
    //printf("You entered: %s\n", buffer);
    printf("\nSending to the server\n");
    n = write(client_fd,buffer,2048,0);
    if (n < 0){
        perror("ERROR reading from socket");
    }
    
    n = read(client_fd, buffer, 2048, 0);
    if (n < 0){
        perror("ERROR reading from socket");
    }
    
    printf("Data received in client: %s\n",buffer);
    
    return 0;
}
