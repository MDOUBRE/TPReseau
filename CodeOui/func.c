#include <stdio.h>
#include <stdlib.h>

//Dans le cas ou on veut nous même choisir les noms des utilisateurs
/*
char *nomUtil(char *tab, int m)
{
	char *tmp;
	tmp=malloc(sizeof(char)*m);
	
	printf("nom utilisateur ?\n");
	scanf("%s",&*tmp);
	
	return tmp;
}
*/

//Initialise la liste avec les prénoms des utilisateurs
char **fctlisteUtil(char **tab, int n, int m){
	
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
	
	/*
	char *tmp;
	tmp=malloc(sizeof(char)*m);
	
	int i,j;
	for(i=0;i<n;i++)
	{
		tmp=nomUtil(tmp,m);
		for(j=0;j<m;j++)
		{
			tab[i][j]=(tmp[j]);	
		}
	}
	*/
	
	return tab;
}

//Affiche la liste des utilisateurs
void afficheListeUtil(char **tab, int n, int m){
	int i, j;
	
	printf("Liste d'utilisateurs : \n");
	for(i=0;i<n;i++)
	{
		printf("- %s",tab[i]);
		printf("\n");
	}
	printf("\n");	
}

	
int main(){

	printf("------------DEBUT------------\n\n");
	int i, j;
	int n, m;
	n=10;	//nombre d'utilisateurs
	m=10;	//nombre de caractères max pour un nom d'utilisateurs
	
	//printf("Combien d'utilisateurs ?\n");
	//scanf("%d",&n);
	
	printf("\n");
	printf("------------UTILS------------\n\n");
	
	char **listeUtil;
	listeUtil=malloc(sizeof(char)*n);	
	for(i=0;i<n;i++)
	{
		listeUtil[i]=malloc(sizeof(char)*m);		
	}	
	
	listeUtil=fctlisteUtil(listeUtil,n,m);
	afficheListeUtil(listeUtil,n,m);
	
	printf("-----------------------------\n\n");
	
	printf("---------VERIF UTILS---------\n\n");
	
	
	printf("-----------------------------\n\n");
	
	printf("-------------FIN-------------\n\n");
	
	return 0;
}
