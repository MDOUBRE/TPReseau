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
	dossier.tabdossier=malloc(sizeof(int)*TAILLEMAX);
	dossier.tabfichier=malloc(sizeof(int)*TAILLEMAX);
	return dossier;
}

//Affiche le contenu du dossier (dossiers et fichiers)
struct Dossier afficheDossier(struct Dossier dossier){
	int i=0;
	printf("%s :\n",dossier.nom);
	
	if(dossier.nbfic==0 && dossier.nbdoss==0)
	{
		printf("le dossier %s est vide.\n",dossier.nom);
	}
	else
	{	
		for(i=0;i<dossier.nbdoss;i++)
		{
			printf("-%s\n",dossier.tabdossier[i].nom);
		}
		for(i=0;i<dossier.nbfic;i++)
		{
			printf("-%s\n",dossier.tabfichier[i].nom);
		}
	}
}

//fct papa pour le dossier2 qui lui met le dossier1 comme parent quand on crée un dossier2 dans un dossier 1

//Ajoute un fichier dans un dossier et incrémente le nombre de fichiers de la strcucture du dossier
struct Dossier ajouteDossier(struct Dossier dossier1, struct Dossier dossier2){
	int i;
	dossier1.tabdossier[dossier1.nbdoss]=dossier2;
	dossier1.nbdoss=dossier1.nbdoss+1;
	return dossier1;
}

//Ajoute un fichier dans un dossier et incrémente le nombre de fichiers de la strcucture du dossier
struct Dossier ajouteFichier(struct Dossier dossier, struct Fichier fichier){
	int i;
	dossier.tabfichier[dossier.nbfic]=fichier;
	dossier.nbfic=dossier.nbfic+1;
	return dossier;
}

// Fonction de chat entre client et serveur
void func(int sockfd) 
{ 
    char buffer[MAX]; 
    int n; 
    
    //Création de notre dossier Racine
    struct Dossier Racine=CreerDossier("Racine");
    //enlever les /**/ en dessous pour que le ls n'affiche pas "vide"
    /*
    struct Fichier fichiertest=CreerFichier("fichiertest");
    Racine=ajouteFichier(Racine,fichiertest);
    struct Dossier SousRacine=CreerDossier("SousRacine");
    Racine=ajouteDossier(Racine,SousRacine);
    */
    
    // boucle infinie 
    while (1) { 
        bzero(buffer, MAX); 
  
        // lit le message du client et le copie dans le buffer 
        read(sockfd, buffer, sizeof(buffer)); 
        // print le buffer qui copntient le contenu du client
        printf("From client: %s\n To client : ", buffer); 
             
        bzero(buffer, MAX); 
        n = 0; 
        // copie le message du serveur dans le buffer 
        while ((buffer[n++] = getchar()) != '\n') 
            ; 
  
        // et envoie le buffer au client 
        write(sockfd, buffer, sizeof(buffer));      

        // si le message contient ls alors affiche tous les dossieres et fichiers contenu dans le repertoire
        if (strncmp("ls", buffer, 2) == 0) { 
            afficheDossier(Racine);
        }  
        
        // si le message contient "Exit"alors le serveur quitte et end la connection 
        if (strncmp("exit", buffer, 4) == 0) { 
            printf("Server Exit...\n"); 
            break; 
        }   
         
    } 
} 

// Main 
int main() 
{ 
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 

    //tests dossiers/fichiers/affichage/ajouts
    /*
    struct Dossier dossier1=CreerDossier("dossier1");
    struct Fichier fichier1=CreerFichier("fichier1");
    struct Fichier fichier2=CreerFichier("fichier2");
    dossier1=ajouteFichier(dossier1,fichier2);
    printf("coucou\n\n");
    afficheDossier(dossier1);
  	printf("\n\n");
    dossier1=ajouteFichier(dossier1,fichier1);
    afficheDossier(dossier1);
    printf("\n\n");
    */
    
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
