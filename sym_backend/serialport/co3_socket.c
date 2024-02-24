//
// Created by lcm on 2/12/24.
//
#include "co3_socket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

int initTCPSocket(const char * port_str){
    int sockfd = socket(AF_INET , SOCK_STREAM , 0);
    int portno = atoi(port_str);
    struct hostent *server;
    struct sockaddr_in serv_addr;
    char * hostname = "localhost";
    if(sockfd == -1){
        fprintf(stderr, "cannot create socket");
        abort();
    }
    server = gethostbyname(hostname);
    if(server == NULL){
        fprintf(stderr, "Error, no such host as %s", hostname);
        abort();
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        fprintf(stderr, "error connecting to a port");
        abort();
    }
    return sockfd;
}