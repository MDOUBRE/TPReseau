#include <netdb.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 

void func(int sockfd) 
{ 
    char buff[MAX]; 
    int n; 
    while(1) { 
        bzero(buff, sizeof(buff)); 
        printf("Entrer une commande : "); 
        n = 0; 
        while ((buff[n++] = getchar()) != '\n'); 
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("Reponse du serveur : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0) { 
            printf("Client Exit...\n"); 
            break; 
        } 
    } 
} 
  
int main() 
{ 
    int sockfd; 
    struct sockaddr_in servaddr; 
  
    // creation de socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("Creation de socket rate\n"); 
        exit(0); 
    } 
    else
        printf("Socket cree\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 
  
    // connection des socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection au serveur rate\n"); 
        exit(0); 
    } 
    else
        printf("connecte au serveur\n"); 
  
    // appel de la fonction pour les echange client serveur
    func(sockfd); 
  
    // fermeture de la socket
    close(sockfd); 
} 
