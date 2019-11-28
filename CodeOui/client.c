#include <netdb.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#define MAX 500 
#define PORT 8080 
#define SA struct sockaddr 

void func(int sockfd) 
{ 
    char buff[MAX]; 
    FILE *f;
    int n;
    char c;
    int lecture=0; 
    while(1) { 
        bzero(buff, sizeof(buff)); 
        printf("Entrer une commande (liste pour la liste des commandes) : "); 
        n = 0; 
        printf("test-2");
        //while ((buff[n++] = getchar()) != '\n'); //gros seg fault la j'ai essayee ce qu'on avait de base le scanf le fgets de malloc le buff aussi
        scanf("%s\n",buff);
        
        printf("test-1");
        if ((strncmp(buff, "put", 3)) == 0) { 
            printf("yo tu veux rajouter un fichier\n"); 
            char namefile[25];
            int i=4;
            for(i=4;i<strlen(buff);i++){ // on recupere le nom du fichier
				namefile[i-3]=buff[i];
			}
			printf(namefile);
            if((f = fopen(namefile,"r"))==NULL){
				printf("impossible d'ouvrir le fichier en lecture");
				break;
			}
			write(sockfd, buff, sizeof(buff)); // on envoie la commande pour que le serveur sache qu'il doit attendre un fichier
			fscanf(f,"%s",buff); // on met le fichier dans le buffer qui sera envoyer par la suite
        }
        printf("test");
        write(sockfd, buff, sizeof(buff));
        if ((strncmp(buff, "get", 3)) == 0) { 
			printf("yo tu veux telecharger un fichier\n"); 
			lecture=1;
            char namefile[25];
            int i=4;
            for(i=4;i<strlen(buff);i++){ // on recupere le nom du fichier
				namefile[i-3]=buff[i];
			}
			printf(namefile);
            if((f = fopen(namefile,"w"))==NULL){
				printf("impossible d'ouvrir le fichier en ecriture");
				break;
			}
		}
        bzero(buff, sizeof(buff));
        printf("test2");
        read(sockfd, buff, sizeof(buff));
        printf("test3");
        if(lecture==1){
			fprintf(f,"%s",buff); // on créé le fichier a partir du buffer
			lecture=0;
		}
		fclose(f);
		printf("test4");
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
