#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#define PORT 55555

int main (int argc, char *argv[]) {
    char buffer[512];
    int soc = socket(AF_INET, SOCK_STREAM, 0);

    struct hostent *host_inf;
    struct sockaddr_in server;

    //vérification de la valididé des arguments
    if (argc != 3) {    //TODO better verification
        printf("Incorrect arguments usage: ./client [server ip] [port]\n");
        exit(1);
    }
    host_inf = gethostbyname(argv[1]);

    

    //memcpy(&server.sin_addr.s_addr,host_inf->h_addr, host_inf->h_length);
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    server.sin_addr = *((struct in_addr *)host_inf->h_addr);

    //creation d'un socket
    if ((soc = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(1);
    }

    //connexion  au serveur
    if (connect(soc, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1){
        perror("connect");
        exit(1);
    }

    //dialogue serveur
    while (1) {
        printf("Entrez une référence (q pour quitter): ");
        scanf("%s", buffer);
        write(soc, buffer, strlen(buffer));

        if (!strcmp(buffer, "q")) break;
        
        bzero(buffer, sizeof(buffer));
        read(soc, buffer, 512);
        printf("%s", buffer);
    }

    //fermeture serveur
    shutdown(soc,2);
    close(soc);
 
    return 0; 
}