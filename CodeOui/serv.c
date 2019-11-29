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
  
/*  
struct Fichier
{
    char *nom;
};

struct Dossier
{
    char *nom;
    int nbfic;
    int nbdoss;
    struct Dossier *tabdossier;
    struct Fichier *tabfichier;
    //struct Dossier papa;
};

struct Fichier CreerFichier(char *tab){
	struct Fichier fichier;
	fichier.nom=tab;
	return fichier;
}

struct Dossier CreerDossier(char *tab){
	struct Dossier dossier;
	dossier.nom=tab;
	dossier.nbfic=0;
	dossier.nbdoss=0;
	dossier.tabdossier=malloc(sizeof(int)*MAX);
	dossier.tabfichier=malloc(sizeof(int)*MAX);
	return dossier;
}

//Affiche le contenu du dossier (dossiers et fichiers)
void afficheDossier(struct Dossier dossier, char* chaineARenvoi){
	int i=0;
	sprintf(chaineARenvoi,"%s :\n",dossier.nom);
	if(dossier.nbfic==0 && dossier.nbdoss==0)
	{
		sprintf(chaineARenvoi,"%s le dossier %s est vide.\n",chaineARenvoi, dossier.nom);
	}
	else
	{	
		for(i=0;i<dossier.nbdoss;i++)
		{
			sprintf(chaineARenvoi,"%s -%s\n",chaineARenvoi,dossier.tabdossier[i].nom);
		}
		for(i=0;i<dossier.nbfic;i++)
		{
			sprintf(chaineARenvoi,"%s -%s\n",chaineARenvoi,dossier.tabfichier[i].nom);
		}
	}
	printf("%s\n",chaineARenvoi);
}


//fct papa pour le dossier2 qui lui met le dossier1 comme parent quand on crée un dossier2 dans un dossier 1

//Ajoute un fichier dans un dossier et incrémente le nombre de fichiers de la strcucture du dossier
struct Dossier ajouteDossier(struct Dossier dossier1, struct Dossier dossier2){
	//int i;
	dossier1.tabdossier[dossier1.nbdoss]=dossier2;
	dossier1.nbdoss=dossier1.nbdoss+1;
	return dossier1;
}

//Ajoute un fichier dans un dossier et incrémente le nombre de fichiers de la strcucture du dossier
struct Dossier ajouteFichier(struct Dossier dossier, struct Fichier fichier){
	//int i;
	dossier.tabfichier[dossier.nbfic]=fichier;
	dossier.nbfic=dossier.nbfic+1;
	return dossier;
}
*/

// Donne le nom du dossier destination à parti de la commande rentrée
char* getDestination(char *tab){
	//char tabcd[2];
	//char lettre;
	
	char *destination=malloc(sizeof(int)*MAX);
	int i=3;
	//int j=0;
	while(i!=MAX)
	{
		destination[i-3]=tab[i];
		i=i+1;
	}
	printf("%s\n",destination);
	return destination;	
}

// Remplace dans dossEnCours par le dossier destination demandé
/*
struct Dossier changeDossEnCours(struct Dossier dossier){
	struct Dossier changement=CreerDossier("changement");
	changement.nom=dossier.nom;
	changement.nbfic=dossier.nbfic;
	changement.nbdoss=dossier.nbdoss;
	changement.tabdossier=dossier.tabdossier;
	changement.tabfichier=dossier.tabfichier;
	printf("le nom changé est : %s\n",changement.nom);
	return changement;
}
*/


int compare(char *tab, char *tab2){
	int i=0;
	int j=0;
	int result=-1;
	for(i=0;i<20;i++){
		printf("%c - %c\n",tab[i],tab2[i]);
		if(tab[i]==tab2[i]){
				result=0;
		}
		else
		{
			result=-1;
		}
	}
	return result;
}


// Cherche le dossier demandé tmp dans dans le dossier en cours dossEnCours
/*
int chercheDoss(struct Dossier dossEnCours,char *tmp)
{
	int result=-1;
	int i;
	char *tmp2=malloc(sizeof(int)*MAX);
	printf("le dossEnCours est %s\n",dossEnCours.nom);
	printf("%s a %d dossiers\n",dossEnCours.nom,dossEnCours.nbdoss);
	for(i=0;i<dossEnCours.nbdoss;i++)
	{
		printf("on est dans le for\n");
		tmp2=dossEnCours.tabdossier[i].nom;
		printf("tmp2 : %s\n",tmp2);
		printf("tmp : %s\n",tmp);
		//a vérifier car ne fait pas la comparaison ou du moins ne renvoie pas 0 alors que devrait
		int coucou=compare(tmp,tmp2);
		printf("comparaison : %d\n",coucou);
		if(compare(tmp,tmp2)==0)
		{
			printf("on est dans result\n");
			result=0;
		}
	}
	printf("result=%d\n",result);
	return result;
}
*/

// Renvoie le dossier demandé tmp à parti du dossier en cours dossEnCours
/*
struct Dossier donneDoss(struct Dossier dossEnCours,char *tmp){
	int i=0;
	char *tmp2;
	
	for(i=0;i<dossEnCours.nbdoss;i++)
	{
		tmp2=dossEnCours.tabdossier[i].nom;
		printf("tmp2 : %s\n",tmp2);
		if(strcmp(tmp,tmp2)==0)
		{
			return dossEnCours.tabdossier[i];
		}
	}
	return dossEnCours;
}
*/

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


// Fonction de chat entre client et serveur
void func(int sockfd) 
{ 
    //char *buffer=malloc(sizeof(int)*MAX); 
    char buffer[MAX];
        
    //Création de notre dossier Racine
    /*
    struct Dossier Racine=CreerDossier("Racine");    
    struct Fichier fichiertest=CreerFichier("fichiertest");
    Racine=ajouteFichier(Racine,fichiertest);
    struct Dossier SousRacine=CreerDossier("SousRacine");
    Racine=ajouteDossier(Racine,SousRacine);
        
    struct Dossier dossEnCours;
    dossEnCours=CreerDossier("dossEnCours");
    dossEnCours=changeDossEnCours(Racine);
    
    */
    
    // boucle infinie 
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
            
            /*
            tmp=getDestination(buffer);
            printf("la destination est %s\n",tmp);
            if(chercheDoss(dossEnCours,tmp)==0){
            	dossEnCours=changeDossEnCours(donneDoss(dossEnCours,tmp));
            	//efface le buffer
				bzero(buffer, MAX);
				sprintf(buffer,"dosier changee : %s",dossEnCours.nom);
            	write(sockfd, buffer, sizeof(buffer));
            }
            else{
            	//efface le buffer
				bzero(buffer, MAX);
				sprintf(buffer,"Vous ne pouvez pas accéder à ce dossier à partir de %s\n",dossEnCours.nom);	
            	write(sockfd, buffer, sizeof(buffer));
            }
            */
        }  
        
        // si le message contient ls alors affiche tous les dossieres et fichiers contenu dans le repertoire
        if (strncmp("ls", buffer, 2) == 0) { 
			//efface le buffer
			bzero(buffer, MAX);
            tmp=listdir(".");
            sprintf(buffer,tmp,MAX);
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

        
        
        // si le message contient "Exit"alors le serveur quitte et end la connection 
        if (strncmp("exit", buffer, 4) == 0) { 
            printf("Server Exit...\n"); 
            break; 
        }   
        */
         
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
  
    // Accept the data packet from client and verification 
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
