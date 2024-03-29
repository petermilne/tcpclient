/* 
 * tcpclient.c - A simple TCP client
 * usage: tcpclient <host> <port>
 * original: https://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/tcpclient.c
 * Adapted to connect to a port and "read forever"
 */

/* pgm hacked from 
 * https://www.binarytides.com/server-client-example-c-sockets-linux/
 *
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define BUFSIZE 1024

/* 
 * error - wrapper for perror
 */
void error(char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char **argv) {
    int sockfd, portno, n;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];
    int writer = 0;

    /* check command line arguments */
    if (argc < 3) {
       fprintf(stderr,"usage: %s <hostname> <port> [0:read 1: write]\n", argv[0]);
       exit(0);
    }
    hostname = argv[1];
    portno = atoi(argv[2]);
    if (argc == 4){
        writer = atoi(argv[3]);
    }

    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }

    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
	  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);

    /* connect: create a connection with the server */
    if (connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) 
      error("ERROR connecting");

    if (writer == 0){
        while((n = read(sockfd, buf, BUFSIZE)) > 0){
	    write(1, buf, n);
        }
    }else{
        while((n = read(0, buf, BUFSIZE)) > 0){
            int nw = write(sockfd, buf, n);
            if (nw < n){
                error("ERROR writing to socket");
                break;
            }
        }
    }
    close(sockfd);
    return 0;
}
