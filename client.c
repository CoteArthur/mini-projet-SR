#include <sys/types.h>          
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h> 

#define ip "127.0.0.1"
#define port 2000

//declaration et creation de socket de type 


void main(void)
{
    
char *hostname;

struct sockaddr_in server ;
struct hostent *host_inf ;
hostinf=gethostbyname(hostname);

SOCK_STREAM soc ; 
if((soc =socket(AF_INET, SOCK_STREAM, 0)==-1){
    printf("erreur");
    exit(0);
}
 server.sin_addr.s
 server.sin_family=AF_INET;
 server.sin_port=htons(PORT)



server.sin_addr = *(IN_ADDR *) hostinfo->h_addr;

 //demande de conection



}