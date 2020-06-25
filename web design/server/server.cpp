#include <cstdio>   
#include <cstring>         
#include <cstdlib>       
#include <unistd.h>          
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <pthread.h> 
#include "../../backend/interpreter.hpp"

using namespace sjtu;
bool cleaned = false;

#define MAXBUFF 16384

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
    void run_once(bool with_backend) {
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
            fflush(stdout);
            if (with_backend) {
                printf("pending\n");
                inpterpreter(buff);
                printf("send msg to client >>> %s\n", buff);
            } else {
                printf("send msg to client >>> ");
                scanf("%s", buff);
            }
            n = strlen(buff);
            fflush(stdout);
            buff[n] = '\0';
            send(client_sockfd, buff, n, 0);
            close_flag = true;    
        }
        close(client_sockfd);
    }
    void run(bool with_backend) {
        run_flag = true;
        while (run_flag) {
            run_once(with_backend); 
        }
    }
};



int main(int argc, char** argv){
    Server<Interpreter> server;
    if (argc == 1)
        server.run(true);
    else
        server.run(false);
    return 0;
}