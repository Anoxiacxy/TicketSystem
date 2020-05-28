#include <stdio.h>   
#include <string.h>         
#include <stdlib.h>       
#include <unistd.h>          
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <pthread.h> 

#define MAXBUFF 4096

class Server {
private:
    int  listenfd, connfd;
    struct sockaddr_in  servaddr;
    char  buff[MAXBUFF];
public:
    Server() {
        if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
            printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        }
    }
    Server () {
        
    }
    ~Server() {
        close(listenfd);
    }
    run_forever() {

    }
    run_once() {

    }

}

void getBackendOutput(char * buff) {
    printf("send msg to client >>> ");
    scanf("%s", buff);
    printf("\n");
}

int main(int argc, char** argv){
    
    
    

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(6666);

    if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }

    if( listen(listenfd, 10) == -1){
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }

    printf("======waiting for client's request======\n");
    while(1){
        if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){
            printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
            continue;
        }
        n = recv(connfd, buff, MAXLINE, 0);
        buff[n] = '\0';
        printf("recv msg from client <<< %s\n", buff);
        getBackendOutput(buff);
        
        if ( send(connfd, buff, strlen(buff), 0) < 0 ) {
            printf("send socket error: %s(errno: %d)",strerror(errno),errno);
        }
        close(connfd);
    }
    
    return 0;
}