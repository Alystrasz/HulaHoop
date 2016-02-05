#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>


#include "socket.h"

int socket_serveur;

int creer_serveur(int port)
{
  /* Création de la socket serveur */
  if((socket_serveur = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("socket_serveur");
    //TODO gestion des erreurs
    return -1;
  }

  /* Configuration de la socket serveur */
  struct sockaddr_in saddr ;
  saddr.sin_family      = AF_INET ;     /* Socket ipv4 */
  saddr.sin_port        = htons(port);  /* Port d ’ écoute */
  saddr.sin_addr.s_addr = INADDR_ANY;   /* écoute sur toutes les interfaces */
  
  int optval = 1;
  if(setsockopt(socket_serveur, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
  {
    perror ( "Can not set SO_REUSEADDR option");
    return -1;
  }
  
  if(bind(socket_serveur, ( struct sockaddr *)&saddr, sizeof ( saddr )) == -1)
  {
    perror("bind socket_serveur");
    //TODO gestion des erreurs
    return -1;
  }
  
  /* Utilisation de la socket serveur 
     Attend les connexions sur le port 'port' */
  
  if(listen(socket_serveur, 10) == -1)
  {
    perror("listen socket_serveur");
    //TODO gestion des erreurs
    return -1;
  }
  return 0;
}
