#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define QUEUE_SIZE 10

void end_child();
int init(int *, int);
void service(int);
char *get_by_ref(int);

int main(int argc, char *argv[])
{
    int server_fd;

    //vérification de la valididé des arguments
    if (argc != 2) {    //TODO check if argv[1] is valid port
        printf("Incorrect arguments usage: ./server [port]\n");
        exit(1);
    }
    
    if (init(&server_fd, atoi(argv[1])) == -1)
        exit(1);

    service(server_fd);

    return 0;
}

void end_child() {
    while (wait(NULL)!=-1);
}

int init(int *server_fd, int NO_PORT) {
    struct sockaddr_in server_addr;
    
    //gestion processus fils
    struct sigaction ac;

    ac.sa_handler = end_child;
    ac.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &ac, NULL);

    //création d'une socket d'écoute
    if ((*server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return -1;
    }

    //préparation des champs sin_family, sin_port et sin_addr.s_addr de server_addr variable 
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr =  htonl(INADDR_ANY);
    server_addr.sin_port = htons(NO_PORT);

    //attachement d'une socket à une adresse
    if (bind(*server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        return -1;
    }

    //ouverture de service
    if (listen(*server_fd, QUEUE_SIZE) == -1) {
        perror("listen");
        return -1;
    }

    return 0;
}

void service(int server_fd) {
    int client_fd, sockaddr_in_size = sizeof(struct sockaddr_in);
    struct sockaddr_in client_addr;
    char buffer[512];

    while(1) {
        //acceptation d'une connexion
        if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &sockaddr_in_size)) < 0) {
            perror("accept");
            //continue;
        }

        //création d'un processus fils
        if (!fork()) {
            while (1) {
                //lecture d'une chaine de charactères
                if (read(client_fd, buffer, 512) == -1) {
                    perror("read");
                    break;
                }
                printf("Buffer: %s", buffer);

                if (!strcmp(buffer, "q")) break;

                char *book = get_by_ref(atoi(buffer));
                if (write(client_fd, book, strlen(book)) == -1) {
                    perror("write");
                    break;
                }
            }

            //fermeture du socket client
            close(client_fd);
            exit(0);
        }

        //fermeture du socket client
        close(client_fd);
    }
}

char *get_by_ref(int ref) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;

    if ((fp = fopen("fichier-exemple.txt", "r")) == NULL)
        return "Couldn't open file";

    while ((getline(&line, &len, fp)) != -1) {
        if (atoi(strtok(line, "#")) == ref) {
            return strtok(NULL, "");
        }
    }

    free(line);
    return "Reference not found\n";
}
