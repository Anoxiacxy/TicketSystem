#include <cstdio>   
#include <cstring>         
#include <cstdlib>       
#include <unistd.h>          
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <pthread.h> 
#include "../../backend/interpreter.hpp"

#define MAXBUFF 4096

template<class Interpret>
class Server {
private:
    int  server_sockfd;
    Interpret inpterpreter;
    bool run_flag;
public:
    Server(int MYPORT = 12306, int QUEUESIZE = 10) {
        server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in server_sockaddr;
        server_sockaddr.sin_family = AF_INET;
        server_sockaddr.sin_port = htons(MYPORT);
        server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        socklen_t server_socklen = sizeof(server_sockaddr);

        bind(server_sockfd, (struct sockaddr*)&server_sockaddr, server_socklen);
        listen(server_sockfd, QUEUESIZE);
    }
    ~Server() {
        close(server_sockfd);
    }
    void run_once() {
        int client_sockfd;
        struct sockaddr_in client_sockaddr;
        socklen_t client_socklen = sizeof(client_sockaddr);
        char buff[MAXBUFF];
        
        printf("---------------- waiting for connection ----------------\n");

        client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_sockaddr, &client_socklen);
        
        bool close_flag = false;

        while (!close_flag) {
            int n = recv(client_sockfd, buff, MAXBUFF, 0);
            buff[n] = '\0';
            printf("recv msg from client <<< %s\n", buff);
            inpterpreter(buff);
            printf("send msg to client >>> %s\n", buff);
            send(client_sockfd, buff, strlen(buff), 0);
            close_flag = true;    
        }
        close(client_sockfd);
    }
    void run() {
        run_flag = true;
        while (run_flag) {
            run_once(); 
        }
    }
};



int main(int argc, char** argv){
    Server<Interpreter> server;
    server.run();
    return 0;
}