#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
    char buffer[512];
    int sock;
    int client_socket;    
    int mon_address_longueur, lg;

    struct sockaddr_in mon_address, client_address;

    bzero(&mon_address,sizeof(mon_address));    //met à 0 tous les octets de mon_address
    mon_address.sin_port = htons(30000);        //port 30000
    mon_address.sin_family = AF_INET;
    mon_address.sin_addr.s_addr = htonl(INADDR_ANY);

    /* creation du socket */
    sock=socket(AF_INET,SOCK_STREAM,0);
    if (sock==-1)
    {
        printf("problème de création\n");
        exit(0);
    }
    signal(SIGINT,"Fin");
    

    /* bind serveur - socket */
    int ret;
    ret=bind(sock,(struct sockaddr *)&mon_address,sizeof(mon_address));
    if(ret<0)
    {
        printf("error\n");
    }

    /* ecoute sur la socket */
    listen(sock,5);

    /* accept la connexion */
    //mon_address_longueur = sizeof(client_address); 
    while(1)
    {
        client_socket = accept(sock,(struct sockaddr *)&client_address,sizeof(client_address));

        if (fork() == 0)
        {
            close(sock);

            lg = read(client_socket,buffer, 512);
            printf("le serveur a recu: %s\n",buffer);
            sprintf(buffer,"%s du serveur",buffer);
            write(client_socket,buffer, 512);
            shutdown(client_socket,2);
            close(client_socket);
            exit(0);
        }
    }
    shutdown(sock,2);
    close(sock);
    return 0;
}