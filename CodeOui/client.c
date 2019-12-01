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
		if(entry->d_type != DT_DIR)
		{
			strcat(tab,entry->d_name);
			strcat(tab,"\n");
		}
		else
		{
			strcat(tab,"*");
			strcat(tab,entry->d_name);
			strcat(tab,"\n");
		}
	}
	
	closedir(dp);
	return tab;
}

int testApart(char* name, char* path)
{
	struct dirent *entry;
	DIR *dp;
	int retour=-1;
	int i=0;
	dp=opendir(path);
	if(dp==NULL)
	{
		perror("opendir");
	}

	while((entry=readdir(dp)))
	{
		if((strcmp(entry->d_name,name)==0) && (entry->d_type == DT_DIR)) // if(entry->d_type != DT_DIR){
		{
			retour=0;
		}
	}
	
	closedir(dp);
	return retour;
}

void removespace(char* line)
{
    int i, j;
    for(i = 0; line[i] != '\0'; ++i)
    {
        while (!( (line[i] >= 33 && line[i] <= 123) || line[i] == '\0'))
        {
            for(j = i; line[j] != '\0'; ++j)
            {
                line[j] = line[j+1];
            }
            line[j] = '\0'; 
        }
    }
}

void getNameFile(char* reponse, char* message, int nbCharAOublier)
{
	int i=nbCharAOublier;
	while(i<strlen(message) && message[i]!='\0' && message[i]!=' ')
	{
		reponse[i-nbCharAOublier]=message[i];
		i++;
	}
	removespace(reponse);
}

void fonction(int sockfd) 
{ 
    char buffer[MAX]; 
    FILE *f;
    char path[MAX]=".";
    int n;
    int lecture=0;
    int ouvert=0;
    char prenom[20];

    bzero(buffer,MAX);
    read(sockfd, buffer, MAX);
    printf("%s\n",buffer);
	bzero(buffer,MAX);
	bzero(prenom,20);
	scanf("%s",prenom);
	strcat(buffer,prenom);
    write(sockfd,buffer,MAX);
    read(sockfd,buffer,MAX);
    if ((strncmp(buffer, "exit", 4)) == 0)
    {
		printf("Client Exit...\n"); 
		return; 
	}
	fflush(stdout);

    while(1)
    { 
        bzero(buffer, MAX); 
        printf("Entrer une commande (liste pour la liste des commandes) : \n"); 
        n = 0; 
		char *tmp=malloc(sizeof(int)*MAX);

        //while ((buffer[n++] = getchar()) != '\n'); //gros seg fault la j'ai essayee ce qu'on avait de base le scanf le fgets de malloc le buffer aussi
        while ((buffer[n++] = getchar()) != '\n');
        
		if ((strncmp(buffer, "lls", 3)) == 0)
		{ 
			bzero(buffer, MAX);
            tmp=listdir(path);
            sprintf(buffer,tmp,MAX);
            printf("%s\n",buffer);
        }
        else if ((strncmp(buffer, "lcd", 3)) == 0)
        {
        	char namefile[MAX];
        	bzero(namefile, MAX);
            getNameFile(namefile,buffer,4);
            if(strcmp(namefile,"..")==0 || strcmp(namefile,".")==0)
            {
				if(strlen(path)>2 && strcmp(namefile,".")!=0)
				{
					int i;
					for(i=strlen(path);path[i]!='/';--i)
					{
						path[i]=' ';	
					}
					path[i]='\0';
					bzero(buffer, MAX);
					strcat(buffer,"cd fait\n");
					printf("%s\n",buffer);
				}
				else
				{
					bzero(buffer, MAX);
					strcat(buffer,"cd impossible\n");
					printf("%s\n",buffer);
				}
			}
			else
			{
				if(testApart(namefile,path)==0)
				{
					strcat(path,"/");
					strcat(path,namefile);
					bzero(buffer, MAX);
					strcat(buffer,"cd fait\n");
					printf("%s\n",buffer);
				}
				else
				{
					bzero(buffer, MAX);
					strcat(buffer,"dossier inconnu\n");
					printf("%s\n",buffer);
				}
			}
			
		}
		else if((strncmp(buffer, "lpwd", 4)) == 0)
		{
			bzero(buffer, MAX);
			strcat(buffer,path);
			strcat(buffer,"\n");
			printf("%s\n",buffer);
		}
		else
		{
			if ((strncmp(buffer, "put", 3)) == 0)
			{ 
				ouvert=1;
				char namefile[MAX];
				char dest[MAX];
				bzero(namefile, MAX);
				bzero(dest,MAX) ;
				getNameFile(namefile,buffer,4);
				printf("%s\n",namefile);
				strcat(dest,path);
				strcat(dest,"/");
				strcat(dest,namefile);
				if((f = fopen(dest,"r"))==NULL)
				{
					printf("impossible d'ouvrir le fichier en lecture");
					break;
				}
				write(sockfd, buffer, MAX); // on envoie la commande pour que le serveur sache qu'il doit attendre un fichier
				while(fgets(buffer,MAX,f)!=NULL)
				{
					write(sockfd, buffer, MAX);
					bzero(buffer, MAX); 
				} 
				bzero(buffer, MAX);
				strcat(buffer,"fin");
			}
			
			write(sockfd, buffer, MAX);
			
			if ((strncmp(buffer, "get", 3)) == 0)
			{ 
				ouvert=1;
				lecture=1;
				char namefile[MAX];
				char dest[MAX];
				bzero(dest,MAX) ;
				bzero(namefile, MAX); 
				getNameFile(namefile,buffer,4);
				printf("%s\n",namefile);
				strcat(dest,path);
				strcat(dest,"/");
				strcat(dest,namefile);
				if((f = fopen(dest,"w"))==NULL)
				{
					printf("impossible d'ouvrir le fichier en ecriture");
					break;
				}
			}

			bzero(buffer, MAX);
			read(sockfd, buffer, MAX); 
			if(lecture==1){
				while((strncmp(buffer, "fin", 3)) != 0)
				{
					fputs(buffer,f);
					bzero(buffer, MAX);
					read(sockfd, buffer, MAX);
				}
				lecture=0;
			}
			if(ouvert==1)
			{
				fclose(f);
				ouvert=0;
			}
			printf("Reponse du serveur :\n %s\n", buffer);
			if ((strncmp(buffer, "exit", 4)) == 0)
			{
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
    if (sockfd == -1)
    { 
        printf("Creation de socket rate\n"); 
        exit(0); 
    } 
    else
    {
        printf("Socket cree\n"); 
    }
    bzero(&servaddr, sizeof(servaddr)); 
  
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 
  
    // connection des socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0)
    { 
        printf("connection au serveur rate\n"); 
        exit(0); 
    } 
    else
    {
        printf("connecte au serveur\n"); 
  	}
  	
    // appel de la fonction pour les echange client serveur
    fonction(sockfd); 
  
    // fermeture de la socket
    close(sockfd); 
} 
