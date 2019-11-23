#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define MAX 80 
#define PORT 8080 
#define TAILLEMAX 100
#define SA struct sockaddr 
  
struct Dossier
{
    char nom[20];
    struct Dossier[TAILLEMAX];
    struct Fichier[TAILLEMAX];
};

struct Fichier
{
    char nom[20];
};

// Fonction de chat entre client et serveur
void func(int sockfd, struct Dossier *tab) 
{ 
    char buffer[MAX]; 
    int n; 
    // boucle infinie 
    while (1) { 
        bzero(buffer, MAX); 
  
        // lit le message du client et le copie dans le buffer 
        read(sockfd, buffer, sizeof(buffer)); 
        // print le buffer qui copntient le contenu du client
        printf("From client: %s\t To client : ", buffer); 
             
        bzero(buffer, MAX); 
        n = 0; 
        // copie le message du serveur dans le buffer 
        while ((buffer[n++] = getchar()) != '\n') 
            ; 
  
        // et envoie le buffer au client 
        write(sockfd, buffer, sizeof(buffer));      

        // si le message contient "Exit"alors le serveur quitte et end la connection 
        if (strncmp("exit", buffer, 4) == 0) { 
            printf("Server Exit...\n"); 
            break; 
        }   

        // si le message contient ls alors affiche tous les dossieres et fichiers contenu dans le repertoire
        if (strncmp("ls", buffer, 2) == 0) { 
            printf("repertoires et fichiers...\n"); 
            break; 
        }   
    } 
} 

// Main 
int main() 
{ 
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 

    //creation d'un dossier test avec un fichier test à l'intérieur
    struct Dossier *tabDoss=malloc(sizeof(int)*TAILLEMAX);
    struct Dossier dossiertest;
    dossiertest.nom="dossiertestnom";
    struct Fichier fichiertest;
    dossiertest.Fichier[0]=fichiertest;
    fichiertest.nom="fichiertestnom";
    tabDoss[0]=dossiertest;

  
    // création et vérification du socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("La création de la socket a échoué...\n"); 
        exit(0); 
    } 
    else
        printf("La socket a été crée avec succès...\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assignation IP et Port à la structure 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    // Bind le socket pour donner l'IP et la verification 
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("le bind de la socket a échoué...\n"); 
        exit(0); 
    } 
    else
        printf("Le bind de la socket a été effectué avec succès...\n"); 
  
    // Maintenant le serveur est prêt à écouter
    if ((listen(sockfd, 5)) != 0) { 
        printf("Ecoute ratée...\n"); 
        exit(0); 
    } 
    else
        printf("Ici la voix...\n"); 
    len = sizeof(cli); 
  
    // Accept the data packet from client and verification 
    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0) { 
        printf("l'accpetation serveur a echoué...\n"); 
        exit(0); 
    } 
    else
        printf("Le serveur accepte le client...\n"); 
  
    // fonction de chat entre client et serveur 
    func(connfd,tabDoss); 
  
    // Fermeture du socket après fin
    close(sockfd); 
} 