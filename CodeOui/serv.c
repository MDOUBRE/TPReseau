#include <stdio.h> 
#include <netdb.h> 
#include <unistd.h>
#include <netinet/in.h> 
#include <stdlib.h>
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include <dirent.h>

#define MAX 500 
#define PORT 8080 
#define TAILLEMAX 100
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
		strcat(tab,entry->d_name);
		strcat(tab,"\n");
	}
	
	closedir(dp);
	return tab;
}

// test d'appartenance d'un dossier au celui dna slequel on est
int testApart(char* name, char* path){
	struct dirent *entry;
	DIR *dp;
	int retour=-1;
	int i=0;
	dp=opendir(path);
	if(dp==NULL)
	{
		perror("opendir");
	}
	printf("name pas a pas \n");
	while(i<strlen(name)){
		printf("%c\n",name[i]);
		i++;
	}
	while((entry=readdir(dp)))
	{
		if(strcmp(entry->d_name,name)==0){
			retour=0;
			printf("je suis passé\n");
		}
		printf("truc pas a pas \n");
		i=0;
		while(i<strlen(entry->d_name)){
			printf("%c\n",entry->d_name[i]);
			i++;
		}
		printf("entry dname : %s\n",entry->d_name);
		printf("name : %s\n",name);
	}
	
	closedir(dp);
	return retour;
}

// fct pour supprimer les espaces
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

// fct pour obtenir le nom d'un fichier
void getNameFile(char* reponse, char* message, int nbCharAOublier){
	int i=nbCharAOublier;
	while(i<strlen(message) && message[i]!='\0' && message[i]!=' '){ // on recupere le nom du fichier
		reponse[i-nbCharAOublier]=message[i];
		i++;
	}
	removespace(reponse);
	printf("longueur message : %d\n",strlen(reponse));
}




// Fonction de réponse du serveur
void func(int sockfd) 
{ 

    char buffer[MAX]; 
    FILE *f;
	char path[MAX]=".";
        
    // boucle infinie pour l'execution du programme
    while (1) { 
        bzero(buffer, MAX);
        char *tmp=malloc(sizeof(int)*MAX);
        
		// lit le message du client et le copie dans le buffer 
        read(sockfd, buffer, MAX); 
        
		// print le buffer qui copntient le contenu du client
        printf("bonsoir\n");
        fflush(stdout); //gros segfault ici si on  remet le printf ?
        
		// si le message contient cd alors vérifie le chemin et va dans le dossier souhaité
        if (strncmp("cd", buffer, 2) == 0) { 
        	printf("buffer : %s\n",buffer);
        	char namefile[MAX];
            getNameFile(namefile,buffer,3);
			if(testApart(namefile,path)==0){
				strcat(path,"/");
				strcat(path,namefile);
			}
			bzero(buffer, MAX);
			printf("ici le path : %s\n",path);
			strcat(buffer,"cd fait\n");
			write(sockfd, buffer, sizeof(buffer));
        }  
        
        // si le message contient ls alors affiche tous les dossieres et fichiers contenu dans le repertoire
        if (strncmp("ls", buffer, 2) == 0) { 
			//efface le buffer
			bzero(buffer, MAX);
            tmp=listdir(path);
            sprintf(buffer,tmp,MAX);
            write(sockfd, buffer, sizeof(buffer));
        }
        
		// renvoie la liste des commandes
        if (strncmp("liste", buffer, 4) == 0) { 
			bzero(buffer, MAX);
			strcat(buffer,"liste des commandes\n-ls\n-cd\n-pwd\n-get nomfichier\nput nomfichier\n");
			write(sockfd, buffer, sizeof(buffer));
		}
		
		// renvoie le dossier dans lequel on est
		if (strncmp("pwd", buffer, 3) == 0) { 
			bzero(buffer, MAX);
			strcat(buffer,path);
			strcat(buffer,"\n");
			write(sockfd, buffer, sizeof(buffer));
		}
		
		// met uin fichier dans le dossier en cours
		if ((strncmp(buffer, "put", 3)) == 0) { 
			printf("yo tu veux rajouter un fichier\n"); 
            char namefile[MAX];
            char dest[MAX];
            bzero(namefile, MAX); 
            bzero(dest, MAX); 
            getNameFile(namefile,buffer,4);
            strcpy(dest,path);
            strcat(dest,"/");
            strcat(dest,namefile);
			printf("%s\n",namefile);
            if((f = fopen(dest,"w"))==NULL){
				printf("impossible d'ouvrir le fichier en ecriture");
				break;
			}
			read(sockfd, buffer, MAX); 
			while((strncmp(buffer, "fin", 3)) != 0){
				fputs(buffer,f);
				printf("oui\n");
				bzero(buffer, MAX);
				read(sockfd, buffer, MAX);
			}
			fclose(f);
			bzero(buffer, MAX);
			strcat(buffer,"ecriture succesfull\n");
			write(sockfd, buffer, sizeof(buffer));
		}
        
		// envoie un fichier au client
        if ((strncmp(buffer, "get", 3)) == 0) { 
            printf("yo tu veux envoyer un fichier\n"); 
            char namefile[MAX];
            char dest[MAX];
            bzero(namefile, MAX); 
            bzero(dest, MAX); 
            getNameFile(namefile,buffer,4);
            strcpy(dest,path);
            strcat(dest,"/");
            strcat(dest,namefile);
            if((f = fopen(dest,"r"))==NULL){
				printf("impossible d'ouvrir le fichier en lecture");
				break;
			}
			bzero(buffer, MAX);
			//write(sockfd, buff, sizeof(buff)); // on envoie la commande pour que le serveur sache qu'il doit attendre un fichier
			fscanf(f,"%s",buffer); // on met le fichier dans le buffer qui sera envoyer par la suite
			write(sockfd, buffer, sizeof(buffer));
		}
        
        //efface le buffer
        
        /*
        bzero(buffer, MAX); 
        n = 0; 
        // copie le message du serveur dans le buffer 
        while ((buffer[n++] = getchar()) != '\n'); 
  
        // et envoie le buffer au client 
        write(sockfd, buffer, sizeof(buffer));      

        */
        
        // si le message contient "Exit"alors le serveur quitte et end la connection 
        if (strncmp("exit", buffer, 4) == 0) { 
            printf("Server Exit...\n"); 
            bzero(buffer, MAX);
			strcat(buffer,"exit");
			write(sockfd, buffer, sizeof(buffer));
            break; 
        }   
    } 
} 



// Main 
int main() 
{ 
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
    
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
  
    // accepte les paquets de données du client
    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0) { 
        printf("l'accpetation serveur a echoué...\n"); 
        exit(0); 
    } 
    else
        printf("Le serveur accepte le client...\n"); 
  
    // fonction de chat entre client et serveur 
    func(connfd); 
  
    // Fermeture du socket après fin
    close(sockfd); 
} 
