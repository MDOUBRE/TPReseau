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

void removespace(char* line){
    int i, j;
    for(i = 0; line[i] != '\0'; ++i)
    {
        while (!( (line[i] >= 'a' && line[i] <= 'z') || (line[i] >= 'A' && line[i] <= 'Z') || line[i] == '\0') )
        {
            for(j = i; line[j] != '\0'; ++j)
            {
                line[j] = line[j+1];
            }
            line[j] = '\0';
        }
    }
}

void getNameFile(char* reponse, char* message, int nbCharAOublier){
	int i=nbCharAOublier;
	while(i<strlen(message) && message[i]!='\0' && message[i]!=' '){ // on recupere le nom du fichier
		reponse[i-nbCharAOublier]=message[i];
		i++;
	}
	removespace(reponse);
	printf("longueur message : %d\n",strlen(reponse));
}

void func(int sockfd) 
{ 
    char buff[MAX]; 
    FILE *f;
    int n;
    int lecture=0;
    int ouvert=0;

    while(1) { 
        bzero(buff, MAX); 
        printf("Entrer une commande (liste pour la liste des commandes) : \n"); 
        n = 0; 
        
        //while ((buff[n++] = getchar()) != '\n'); //gros seg fault la j'ai essayee ce qu'on avait de base le scanf le fgets de malloc le buff aussi
        while ((buff[n++] = getchar()) != '\n');

        if ((strncmp(buff, "put", 3)) == 0) { 
            ouvert=1;
            printf("yo tu veux rajouter un fichier\n"); 
            char namefile[MAX];
            bzero(namefile, MAX); 
            getNameFile(namefile,buff,4);
			printf("%s\n",namefile);
			printf("oui\n");
            if((f = fopen(namefile,"r"))==NULL){
				printf("impossible d'ouvrir le fichier en lecture");
				break;
			}
			write(sockfd, buff, MAX); // on envoie la commande pour que le serveur sache qu'il doit attendre un fichier
			while(fgets(buff,MAX,f)!=NULL){
				printf("oui\n");
				bzero(buff, MAX); 
				write(sockfd, buff, MAX);
			} 
			bzero(buff, MAX);
			strcat(buff,"fin");
        }
        
        write(sockfd, buff, MAX);

        if ((strncmp(buff, "get", 3)) == 0) { 
            ouvert=1;
			printf("yo tu veux telecharger un fichier\n"); 
			lecture=1;
            char namefile[MAX];
            bzero(namefile, MAX); 
            getNameFile(namefile,buff,4);
			printf("%s\n",namefile);
            if((f = fopen(namefile,"w"))==NULL){
				printf("impossible d'ouvrir le fichier en ecriture");
				break;
			}
		}

        bzero(buff, MAX);

        read(sockfd, buff, MAX);
        if(lecture==1){
			fprintf(f,"%s",buff); // on créé le fichier a partir du buffer
			lecture=0;
		}
        if(ouvert==1){
		    fclose(f);
            ouvert=0;
        }
        printf("Reponse du serveur :\n %s\n", buff);
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
    // creation et vérification du socket
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
