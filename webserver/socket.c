#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>

#include <string.h>
#include <signal.h>
#include <unistd.h>


#include "socket.h"

int socket_serveur;

int creer_serveur(int port)
{
  /* Création de la socket serveur */
  if((socket_serveur = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("socket_serveur");
    return EXIT_FAILURE;
  }

  /* Configuration de la socket serveur */
  struct sockaddr_in saddr ;
  saddr.sin_family      = AF_INET ;     /* Socket ipv4 */
  saddr.sin_port        = htons(port);  /* Port d ’ écoute */
  saddr.sin_addr.s_addr = INADDR_ANY;   /* écoute sur toutes les interfaces */
  
  int optval = 1;
  if(setsockopt(socket_serveur, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
  {
    close(socket_serveur);
    perror ("Can not set SO_REUSEADDR option");
    return EXIT_FAILURE;
  }
  
  if(bind(socket_serveur, ( struct sockaddr *)&saddr, sizeof ( saddr )) == -1)
  {
    close(socket_serveur);
    perror("bind socket_serveur");
    return EXIT_FAILURE;
  }
  
  /* Utilisation de la socket serveur 
     Attend les connexions sur le port 'port' */
  
  if(listen(socket_serveur, 10) == -1)
  {
    close(socket_serveur);
    perror("listen socket_serveur");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int simple_get(char *req)
{
  char *ptr;
  //char *separateur = { " " };
  char *buffer;
  
  buffer = strdup(req);
  
  //ptr = strtok(buffer, separateur);
  if(strcmp("GET", (ptr = strtok(buffer, " "))) != 0)
    return 0;
  fprintf(stdout, "%s\n", ptr);
  ptr = strtok(NULL, " ");
  fprintf(stdout, "%s\n", ptr);

  ptr = strtok(NULL, " ");
  
  if(strcmp("HTTP/1.0\r\n", ptr) != 0 || strcmp("HTTP/1.1\r\n", ptr) != 0)
  {
      fprintf(stdout, "%s\n", ptr);

    return 0;
  }
  fprintf(stdout, "lol %s\n", ptr);

  return (strtok(NULL, " ") == NULL);
}

void traitement_signal(int sig)
{
  printf("Signal %d reçu\n",sig);
  /* attend un fils */
  if(sig == SIGCHLD)
  {
    if(waitpid(-1, NULL, 0) == -1)
      perror("waitpid");
  }
  /* ignore les SIGPIPE */
  else if(sig == SIGPIPE)
    if(signal(sig, SIG_IGN) == SIG_ERR)
      perror("signal(SIGPIPE, SIG_IGN)");
}

void initialiser_signaux(void)
{
  struct sigaction sa;
  /* handler */
  sa.sa_handler = traitement_signal;
  /* ensemble des signaux vides */
  sigemptyset(&sa.sa_mask);
  /* option par default */
  sa.sa_flags = SA_RESTART;
  if(sigaction(SIGPIPE, &sa, NULL) == -1)
    perror("sigaction(SIGPIPE)");
  if(sigaction(SIGCHLD, &sa, NULL) == -1)
    perror("sigaction(SIGCHLD)");
}
