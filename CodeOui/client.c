#include <netdb.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>
#include <dirent.h>

#define MAX 500 
#define PORT 8080 
#define SA struct sockaddr 

char *listdir(const char *path)
{
	struct dirent *entry;
	char *tab=malloc(sizeof(int)*MAX);
	DIR *dp;
	
	dp=opendir(path);
	if(dp==NULL)
	{
		perror("opendir");
	}
	
	while((entry=readdir(dp)))
	{
		if(entry->d_type != DT_DIR){
			strcat(tab,entry->d_name);
			strcat(tab,"\n");
		}else{
			strcat(tab,"*");
			strcat(tab,entry->d_name);
			strcat(tab,"\n");
		}
	}
	
	closedir(dp);
	return tab;
}



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
	while(i<strlen(message) && message[i]!='\0' && message[i]!=' '){
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
    char path[MAX]=".";
    int n;
    int lecture=0;
    int ouvert=0;
    char prenom[20];

    bzero(buff,MAX);
    read(sockfd, buff, MAX);
    printf("%s\n",buff);
	bzero(buff,MAX);
	bzero(prenom,20);
	scanf("%s\n",prenom);
	strcat(buff,prenom);
    write(sockfd,buff,MAX);
    read(sockfd,buff,MAX);
    printf("%s\n",buff);


    while(1) { 
        bzero(buff, MAX); 
        printf("Entrer une commande (liste pour la liste des commandes) : \n"); 
        n = 0; 
		char *tmp=malloc(sizeof(int)*MAX);


        //while ((buff[n++] = getchar()) != '\n'); //gros seg fault la j'ai essayee ce qu'on avait de base le scanf le fgets de malloc le buff aussi
        while ((buff[n++] = getchar()) != '\n');
        
		if ((strncmp(buff, "lls", 3)) == 0) { 
			bzero(buff, MAX);
            tmp=listdir(path);
            sprintf(buff,tmp,MAX);
            write(stdout, buff, sizeof(buff));
		}else{
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
					write(sockfd, buff, MAX);
					bzero(buff, MAX); 
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
				while((strncmp(buff, "fin", 3)) != 0){
					fputs(buff,f);
					bzero(buff, MAX);
					read(sockfd, buff, MAX);
				}
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
