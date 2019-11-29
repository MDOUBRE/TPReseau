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

// Donne le nom du dossier destination à parti de la commande rentrée
char* getDestination(char *tab){
	char tabcd[2];
	char *destination=malloc(sizeof(int)*20);
	//char lettre;
	int i=0;
	int j=0;
	while(i!=80)
	{
		if(tab[i]=='c' && tab[i+1]=='d' && tab[i+2]==' ')
		{
			tabcd[0]=tab[i];
			i=i+1;
			tabcd[1]=tab[i];
			i=i+2;
			while(tab[i]!=' ')
			{
				destination[j]=tab[i];
				i++;
				j++;
			}
			break;
		}
		i++;
	}
	printf("%s\n",destination);
	return destination;	
}

// Remplace dans dossEnCours par le dossier destination demandé
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

/*
int compare(char *tab, char *tab2){
	int i=0;
	int result=0;
	while(i!=80){
		if(tab[i]!=tab2[i]){
			result=-1;
		}	
		i++;
	}
	return result;
}
*/

// Cherche le dossier demandé tmp dans dans le dossier en cours dossEnCours
int chercheDoss(struct Dossier dossEnCours,char *tmp)
{
	int result=-1;
	int i;
	char *tmp2;
	printf("le dossEnCours est %s\n",dossEnCours.nom);
	printf("%s a %d dossiers\n",dossEnCours.nom,dossEnCours.nbdoss);
	for(i=0;i<dossEnCours.nbdoss;i++)
	{
		printf("on est dans le for\n");
		tmp2=dossEnCours.tabdossier[i].nom;
		printf("tmp2 : %s\n",tmp2);
		printf("tmp : %s\n",tmp);
		//a vérifier car ne fait pas la comparaison ou du moins ne renvoie pas 0 alors que devrait
		if(strcmp(tmp,tmp2)==0)
		{
			printf("on est dans result\n");
			result=0;
		}
	}
	printf("result=%d\n",result);
	return result;
}

// Renvoie le dossier demandé tmp à parti du dossier en cours dossEnCours
struct Dossier donneDoss(struct Dossier dossEnCours,char *tmp){
	int i=0;
	char *tmp2;
	
	for(i=0;i<dossEnCours.nbdoss;i++)
	{
		tmp2=dossEnCours.tabdossier[i].nom;
		printf("tmp2 : %s\n",tmp2);
		if(tmp2==tmp)
		{
			return dossEnCours.tabdossier[i];
		}
	}
	return dossEnCours;
}


// Fonction de chat entre client et serveur
void func(int sockfd) 
{ 
    char buffer[MAX]; 
    //int n;
    
    //Création de notre dossier Racine
    struct Dossier Racine=CreerDossier("Racine");
    //enlever les /**/ en dessous pour que le ls n'affiche pas "vide"
    
    struct Fichier fichiertest=CreerFichier("fichiertest");
    Racine=ajouteFichier(Racine,fichiertest);
    struct Dossier SousRacine=CreerDossier("SousRacine");
    Racine=ajouteDossier(Racine,SousRacine);
    
    
    struct Dossier dossEnCours;
    dossEnCours=CreerDossier("dossEnCours");
    dossEnCours=changeDossEnCours(Racine);
    
    // boucle infinie 
    while (1) { 
        bzero(buffer, MAX);
        char *tmp;
        
		// lit le message du client et le copie dans le buffer 
        read(sockfd, buffer, MAX); 
        
		// print le buffer qui copntient le contenu du client
        printf("bonsoir");
        fflush(stdout); //gros segfault ici si on  remet le printf ?
        
		// si le message contient cd alors vérifie le chemin et va dans le dossier souhaité
        if (strncmp("cd", buffer, 2) == 0) { 
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
				sprintf("Vous ne pouvez pas accéder à ce dossier à partir de %s\n",dossEnCours.nom);	
            	write(sockfd, buffer, sizeof(buffer));
            }
        }  
        // si le message contient ls alors affiche tous les dossieres et fichiers contenu dans le repertoire
        if (strncmp("ls", buffer, 2) == 0) { 
			//efface le buffer
			bzero(buffer, MAX);
            afficheDossier(dossEnCours,buffer);
            write(sockfd, buffer, sizeof(buffer));
        }
        
        if (strncmp("liste", buffer, 4) == 0) { 
			bzero(buffer, MAX);
			strcat(buffer,"liste des commandes\n-ls\n-cd\n-pwd\n-get nomfichier\nput nomfichier\n");
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
