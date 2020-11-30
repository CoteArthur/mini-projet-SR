#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>

#define no_port 55555
#define queue_size 10

void end_child() {
    while (wait(NULL)!=-1);
}

int main()
{
    int server_fd, client_fd, sockaddr_in_size = sizeof(struct sockaddr_in);
    struct sockaddr_in server_addr, client_addr;
    struct sigaction ac;
    
    ac.sa_handler = end_child;
    ac.sa_flags = SA_RESTART; 
    sigaction(SIGCHLD, &ac, NULL);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    //préparation des champs sin_family, sin_port et sin_addr.s_addr de cette variable 
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr =  htonl(INADDR_ANY);
    server_addr.sin_port = htons(no_port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(server_fd, queue_size) == -1) {
        perror("listen");
        exit(1);
    }

    while(1) {
        if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &sockaddr_in_size)) == -1) {
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