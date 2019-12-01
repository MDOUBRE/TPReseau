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

// test d'appartenance d'un dossier au celui dna slequel on est
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
		if((strcmp(entry->d_name,name)==0) && (entry->d_type == DT_DIR)){ // if(entry->d_type != DT_DIR){
			retour=0;
		}
	}
	
	closedir(dp);
	return retour;
}

// fct pour supprimer les espaces
void removespace(char* line)
{
    int i, j;
    for(i = 0; line[i] != '\0'; ++i){
        while (!( (line[i] >= 33 && line[i] <= 123) || line[i] == '\0')){
            for(j = i; line[j] != '\0'; ++j){
                line[j] = line[j+1];
            }
            line[j] = '\0';
        }
    }
}

// fct pour obtenir le nom d'un fichier
void getNameFile(char* reponse, char* message, int nbCharAOublier)
{
	int i=nbCharAOublier;
	while(i<strlen(message) && message[i]!='\0' && message[i]!=' '){ // on recupere le nom du fichier
		reponse[i-nbCharAOublier]=message[i];
		i++;
	}
	removespace(reponse);
}

char **fctlisteUtil(char **tab, int n, int m)
{
	
	tab[0]="Maxime";
	tab[1]="Nicolas";
	tab[2]="Felix";
	tab[3]="Baptiste";
	tab[4]="Alexis";
	tab[5]="Hugo";
	tab[6]="Marie";
	tab[7]="Louis";
	tab[8]="Julien";
	tab[9]="Gabriel";
	tab[10]="Vincent";
	
	return tab;
}

int compare(char **liste, char *buffer)
{
	int i,j;
	int result=-1;
	for(i=0;i<11;i++){
		if(strcmp(liste[i],buffer)==0)
		{
			result=0;
		}
	}
	return result;
}


// Fonction de réponse du serveur
int func(int sockfd) 
{ 

    char buffer[MAX]; 
    FILE *f;
	char path[MAX]=".";
    int commande=0;
	char **listeUtil;
	int i;
	int n=11;	// nombre de personnes 
	int m=20;	// nombre caractères max par prénom
	listeUtil=malloc(sizeof(char)*n);	
	for(i=0;i<n;i++)
	{
		listeUtil[i]=malloc(sizeof(char)*m);		
	}	
	
	listeUtil=fctlisteUtil(listeUtil,n,m);
	
	bzero(buffer,MAX);
	strcat(buffer,"entrez vorte Prénom");
	write(sockfd,buffer,MAX);
	bzero(buffer,MAX);
	read(sockfd, buffer, MAX);
	if(compare(listeUtil,buffer)==-1)
	{
		printf("Server Exit...\n"); 
		bzero(buffer, MAX);
		strcat(buffer,"exit");
		write(sockfd, buffer, MAX);
		return -1; 
	}
	else
	{
		printf("ok\n");
		bzero(buffer,MAX);
		strcat(buffer,"vous êtes autorisé");
		write(sockfd,buffer,MAX);
	}
    // boucle infinie pour l'execution du programme
    while (1) { 
        bzero(buffer, MAX);
        char *tmp=malloc(sizeof(int)*MAX);
        
		// lit le message du client et le copie dans le buffer 
        read(sockfd, buffer, MAX); 
        
		// print le buffer qui copntient le contenu du client
        fflush(stdout); //gros segfault ici si on  remet le printf ?
        
		// si le message contient cd alors vérifie le chemin et va dans le dossier souhaité
        if (strncmp("cd", buffer, 2) == 0) { 
			commande=1;
        	char namefile[MAX];
        	bzero(namefile, MAX);
            getNameFile(namefile,buffer,3);
            if(strcmp(namefile,"..")==0 || strcmp(namefile,".")==0){
				if(strlen(path)>2 && strcmp(namefile,".")!=0){
					int i;
					for(i=strlen(path);path[i]!='/';--i){
						path[i]=' ';
						
					}
					path[i]='\0';
					bzero(buffer, MAX);
					strcat(buffer,"cd fait\n");
					write(sockfd, buffer, sizeof(buffer));
				}else{
					bzero(buffer, MAX);
					strcat(buffer,"cd impossible\n");
					write(sockfd, buffer, sizeof(buffer));
				}
			}else{
				if(testApart(namefile,path)==0){
					strcat(path,"/");
					strcat(path,namefile);
					bzero(buffer, MAX);
					strcat(buffer,"cd fait\n");
					write(sockfd, buffer, sizeof(buffer));
				}else{
					bzero(buffer, MAX);
					strcat(buffer,"dossier inconnu\n");
					write(sockfd, buffer, sizeof(buffer));
				}
			}
			
			printf("ici le path : %s\n",path);
			
        }  
        
        // si le message contient ls alors affiche tous les dossieres et fichiers contenu dans le repertoire
        if (strncmp("ls", buffer, 2) == 0) { 
			commande=1;
			bzero(buffer, MAX);
            tmp=listdir(path);
            sprintf(buffer,tmp,MAX);
            write(sockfd, buffer, sizeof(buffer));
        }
        
		// renvoie la liste des commandes
        if (strncmp("liste", buffer, 4) == 0) { 
			commande=1;
			bzero(buffer, MAX);
			strcat(buffer,"liste des commandes\n-ls\n-cd\n-pwd\n-get nomfichier\n-put nomfichier\n-exit\n");
			write(sockfd, buffer, sizeof(buffer));
		}
		
		// renvoie le dossier dans lequel on est
		if (strncmp("pwd", buffer, 3) == 0) { 
			commande=1;
			bzero(buffer, MAX);
			strcat(buffer,path);
			strcat(buffer,"\n");
			write(sockfd, buffer, sizeof(buffer));
		}
		
		// met un fichier dans le dossier en cours
		if ((strncmp(buffer, "put", 3)) == 0) { 
			commande=1;
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
			commande=1;
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
			write(sockfd, buffer, MAX);
			while(fgets(buffer,MAX,f)!=NULL){
				write(sockfd, buffer, MAX);
				bzero(buffer, MAX); 
			}
			bzero(buffer, MAX);
			strcat(buffer,"fin");
			write(sockfd, buffer, MAX);
		}
        
        if (strncmp("exit", buffer, 4) == 0) { 
			commande=1;
            printf("Server Exit...\n"); 
            bzero(buffer, MAX);
			strcat(buffer,"exit");
			write(sockfd, buffer, sizeof(buffer));
            break; 
        }   
        if(commande==0){
			bzero(buffer, MAX);
			strcat(buffer,"commande inconnu\n");
			write(sockfd, buffer, sizeof(buffer));
		}
		commande=0;
    }
	return 0;
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
