#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>        
#include <sys/socket.h>
#include <netdb.h>

#define MAX_DATA_SIZE 50

int main(int argc, char *argv[]) {
    struct addrinfo hints;
    struct addrinfo *result,*rp;
    int lfd;
    char message[MAX_DATA_SIZE];
    // to fill the sizeof(struct addrinfo) bytes of memory area pointed by &hints by zero
    memset(&hints,0,sizeof(struct addrinfo));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    if(getaddrinfo(argv[1],argv[2],&hints,&result)!=0) {
        printf("Error in getaddrinfo\n");
        return 0;
    }
    for(rp=result;rp!=NULL;rp=rp->ai_next) {
        lfd=socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol);
        if(lfd==-1) {
            printf("Error in getting file descriptor to a socket");
            continue; // try next address
        }
        if(connect(lfd,rp->ai_addr,rp->ai_addrlen)==0) {
            break;
        }
        close(lfd); // try next address
    }
    if(rp==NULL) {
        printf("Could not connect to server\n");
        return 0;
    }
    freeaddrinfo(result);
    for(;;) {
        printf("Enter message to be sent to server: ");
        if(fgets(message,MAX_DATA_SIZE,stdin)==NULL) {
            printf("Error reading from stdin\n");
            continue;
        }
        if(send(lfd,message,strlen(message),0)==-1) {
            printf("Error in sending message to server\n");
        }
    }
    return 0;
}