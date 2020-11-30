#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <unistd.h>

#define NO_PORT 55555
#define QUEUE_SIZE 10

void end_child();
int init(int *);
void service(int *);

int main()
{
    int server_fd;
    
    if (init(&server_fd) == -1)
        exit(1);

    service(&server_fd);

    return 0;
}

void end_child() {
    while (wait(NULL)!=-1);
}

int init(int *server_fd) {
    struct sockaddr_in server_addr;
    struct sigaction ac;

    ac.sa_handler = end_child;
    ac.sa_flags = SA_RESTART; 
    sigaction(SIGCHLD, &ac, NULL);

    if ((*server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return -1;
    }

    //préparation des champs sin_family, sin_port et sin_addr.s_addr de cette variable 
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr =  htonl(INADDR_ANY);
    server_addr.sin_port = htons(NO_PORT);

    if (bind(*server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        return -1;
    }

    if (listen(*server_fd, QUEUE_SIZE) == -1) {
        perror("listen");
        return -1;
    }
    printf("Listening on port %d", NO_PORT);

    return 0;
}

void service(int *server_fd) {
    int client_fd, sockaddr_in_size = sizeof(struct sockaddr_in);
    struct sockaddr_in client_addr;

    while(1) {
        if ((client_fd = accept(*server_fd, (struct sockaddr *)&client_addr, &sockaddr_in_size)) == -1) {
            perror("accept");
            continue;
        }

        if (!fork()) {  //processus fils
            char * in;
            if (read(client_fd, in, sizeof(in)) == -1)
                perror("read");

            printf(in);

            int out = 1;
            if (write(client_fd, &out, sizeof(int)) == -1)
                perror("write");

            close(client_fd);
            exit(0);
        }
        close(client_fd);
    }
}