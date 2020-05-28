#include <cstdio>   
#include <cstring>         
#include <cstdlib>       
#include <unistd.h>          
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <pthread.h> 
#include "../backend/interpreter.hpp"

#define MAXBUFF 4096

template<class Communicate>
class Server {
private:
    int  server_sockfd;
    Interpret inpterpret;
public:
    Server(int MYPORT = 12306, int QUEUESIZE = 10) {
        server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in server_sockaddr;
        server_sockaddr.sin_family = AF_INET;
        server_sockaddr.sin_port = htons(MYPORT);
        server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        socklen_t server_sockaddr = sizeof(server_sockaddr);

        bind(server_sockfd, (struct sockaddr*)&server_sockaddr, server_sockaddr);
        listen(server_sockfd, QUEUESIZE);
    }
    ~Server() {
        close(server_sockfd);
    }
    run_once() {
        int client_sockfd;
        struct sockadd_in client_sockaddr;
        socklen_t client_sockaddr = sizeof(client_sockaddr);
        char buff[MAXBUFF];
        
        client_sockfd = accept(server_sockfd, (struct sockaddr*)client_sockaddr, &client_sockaddr));
        
        bool close_flag = false;

        while (!close_flag) {
            int n = recv(client_sockfd, buff, MAXBUFF, 0);
            buff[n] = '\0';
            printf("recv msg from client <<< %s\n", buff);
            communicate(buff);
            printf("send msg to client >>> %s\n", buff);
            send(client_sockfd, buff, strlen(buff), 0);
            close_flag = true;    
        }
        close(client_sockfd);
    }
    run() {
        while (run_flag) run_once(); 
    }
}



int main(int argc, char** argv){
    Server server()
    return 0;
}