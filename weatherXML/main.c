//  main.c
//  ECE1150
//
//  Created by Nicholas Sallinger on 9/27/17.
//  Copyright © 2017 Nicholas Sallinger. All rights reserved.
//

#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>


#include <arpa/inet.h>
#define PORT "80" // the port client will be connecting to

#define BUFFER_SIZE 512 // max number of bytes we can get at once

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}



int main(int argc, const char * argv[]) {
    int sockfd = 0;
    char headbuf[BUFFER_SIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];
    
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    if ((rv = getaddrinfo("api.wunderground.com", PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
    
    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }
        
        break;
    }
    
    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }
    
    f(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
      s, sizeof s);
    printf("client: connecting to %s\n", s);
    
    
    char *header = "GET /api/b0a73c25c2f40b1b/conditions/q/PA/Pittsburgh.json HTTP/1.1\r\nHost: api.wunderground.com\r\n\r\n";
    
    send(sockfd, header, strlen(header),0);
    int byte_count;
    byte_count = recv(sockfd,headbuf,sizeof(headbuf)-1,0); // <-- -1 to leave room for a null terminator
    
    
    char* ret;
    ret = strstr(headbuf, "Content-Length:");
    ret = (ret+15);
    int clen = atoi(ret);
    
    
    int arraysize = BUFFER_SIZE + clen;
    char fullarray[arraysize]; // a little too long but so what...
    int i = 0;
    
    strcpy(fullarray, headbuf);
    //do another read and put that into full array of content
    
    int temp = 0;
    char tempbuffer[clen];
    
    while(temp < (clen - 150)){
        byte_count = recv(sockfd,tempbuffer,clen-1,0);
        temp = temp + byte_count;
        strcat(fullarray, tempbuffer);
        
    }
    
    
    freeaddrinfo(servinfo); //being a good programmer
    close(sockfd);
    
    // <full>Pittsburgh, PA</full>
    // <temp_f>65.7</temp_f>
    // <wind_mph>2.2</wind_mph>
    // <feelslike_f>65.7</feelslike_f>
    char * start;
    start = strstr (fullarray,"<full>");
    char* end;
    end = strstr(fullarray,"</full>");
    printf("working:\n");
    for(i = 0; i < (end-start); i++){
        printf("%c",fullarray[i]);
    }
    return 0;
}
