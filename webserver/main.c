#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>

#include "socket.h"

#define PORT 8080

extern int socket_serveur;
int main(void){

  //Création du serveur sur le port PORT
  if(creer_serveur(PORT) == -1)
  {
    perror("Creation du serveur");
    //TODO gestion des erreurs
    return -1;
  }

  //Acceptation d'une connexion 
  int socket_client;
  if((socket_client = accept(socket_serveur, NULL, NULL)) == -1)
  {
    perror("accept");
    //TODO gestion des erreurs
    return -1;
  }
  
  /* On peut maintenant dialoguer avec le client */
  const char* message_bienvenue = "Bonjour , bienvenue sur mon serveur\n ";
  write(socket_client, message_bienvenue, strlen(message_bienvenue));
  
  return 0;
}
