# TPReseau
Utilisation du makefile :
"make client" pour compiler l'executable client
"make serv" pour compiler l'executable serveur
"make clean" pour supprimer les executables

Pour executer il faut d'abord lancer le serveur puis le client. Ensuite le client demandera un nom, si le nom donné fait parti de la liste le client continue avec une connexion au serveur normal sinon il ferme le client et le serveur.

Une fois le serveur et le client lancé toute les commandes ce font coté client
Les commandes :
"ls" affiche le contenu du dossier du serveur
"cd nomDeDossier" change de dossier a nomDeDossier du coté serveur
"pwd" affiche le chemin jusqu'au dossier ouvert du coté serveur
"put nomDeFichier" met un fichier sur le serveur
"get nomDeFichier" telecharge un fichier du serveur
"lls" affiche le contenu du dossier du client
"lcd nomDeDossier" change de dossier a nomDeDossier du coté client
"lpwd" affiche le chemin jusqu'au dossier ouvert du coté client
"exit" ferme le client et le serveur
