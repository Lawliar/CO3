//
// Created by lcm on 2/12/24.
//
#include "co3_socket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/un.h>

char * dir_cstr = NULL; 

int initTCPSocket(int portno){
    struct hostent *server;
    struct sockaddr_in serv_addr;
    char * hostname = "localhost";
    int sockfd = socket(AF_INET , SOCK_STREAM , 0);
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
        fprintf(stderr, "error connecting to a TCP port");
        abort();
    }
    return sockfd;
}



int initUnixSocket(){
    struct sockaddr_un addr;
    if(dir_cstr == NULL){
        fprintf(stderr, "dir_cstr is NULL");
        abort();
    }
    int dir_cstr_len = strlen(dir_cstr);
    if(dir_cstr_len == 0){
        fprintf(stderr, "dir_cstr is empty");
        abort();
    }
    int cur =  dir_cstr_len-1; 
    while(dir_cstr[cur] == '/' && cur >= 0){
        dir_cstr[cur] = '\0';
        cur--;
    }
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "cannot create socket");
        abort();
    }
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, dir_cstr, sizeof(addr.sun_path)-1);
    strncat(addr.sun_path, "/CO3.sock", sizeof(addr.sun_path)-1);
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        fprintf(stderr, "binding to %s failed", addr.sun_path);
        abort();
    }
    return sockfd;
}

int initSock(const char * port_str){
    int sockfd;
    int portno = atoi(port_str);
    if(portno == 0){
        
    }else{
        sockfd = initTCPSocket(port_str);
    }
    
    return sockfd;
}