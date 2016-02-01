#include <sys/types.h>
#include <sys/socket.h>
#include "socket.h"

#define PORT 8080
int creer_serveur(int port)
{
  //Création de la socket serveur
  int socket_serveur ;
  if((socket_serveur = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("socket_serveur");
    //TODO gestion des erreurs
  }

  //Configuration de la socket serveur
  struct sockaddr_in saddr ;
  saddr.sin_family = AF_INET ;        /* Socket ipv4 */
  saddr.sin_port = htons(PORT);      /* Port d ’ écoute */
  saddr.sin_addr.s_addr = INADDR_ANY; /* écoute sur toutes les interfaces */
  if(bind(socket_serveur, ( struct sockaddr *)& saddr, sizeof ( saddr )) == -1)
  {
    perror("bind socker_serveur");
    //TODO gestion des erreurs
  }
  
  /* Utilisation de la socket serveur 
     Attend les connexions sur le port PORT */
  
  if(listen(socket_serveur, 10) == -1)
  {
    perror("listen socket_serveur");
    //TODO gestion des erreurs
  }
  return 0;
}
