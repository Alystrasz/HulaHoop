#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>


#include <fcntl.h>
#include <limits.h>
#include <libgen.h>

#include <string.h>
#include <signal.h>
#include <unistd.h>


#include "socket.h"
#include "stats.h"


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
    perror("Can not set SO_REUSEADDR option");
    return EXIT_FAILURE;
  }
  
  if(bind(socket_serveur, (struct sockaddr *)&saddr, sizeof (saddr)) == -1)
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

char *fgets_or_exit(char *buffer, int size, FILE *stream)
{
  char *buf;
  if((buf = fgets(buffer, size, stream)) == NULL)
    exit(EXIT_FAILURE);
  return buf;
}

int parse_http_request(const char *request_line, http_request *request)
{
  char *methode;
  char *request_target;
  char *http_version;
  char *buffer = strdup(request_line);
  
  /* On recupere les infos a analyser */
  if((methode = strtok(buffer, " ")) == NULL)
    return 0;
  if((request_target = strtok(NULL, " ")) == NULL)
     return 0;
  if((http_version = strtok(NULL, " ")) == NULL)
    return 0;
  
  /* On analyse les infos recupere et on renseigne la struct request */
  if(strcmp(methode, "GET") == 0)
    request->method = HTTP_GET;
  else
    request->method = HTTP_UNSUPPORTED;

  /* une fonction pour analyser l'url? */
  request->url = request_target;

  /* le code pourrait etre factorise pour l'affectation de la version majeur
     mais je prend les devant si jamais il me prend l'envie de le rendre compatible HTTP/2 */
  if(strcmp("HTTP/1.0\r\n", http_version) == 0)
  {
    request->major_version = 1;
    request->minor_version = 0;
  }
  else if(strcmp("HTTP/1.1\r\n", http_version) == 0)
  {
    request->major_version = 1;
    request->minor_version = 1;
  }
  else
    return 0;
  return 1;
}

void skip_headers(FILE *client)
{
  char buf[255];
  while(buf[0] != '\r' && buf[1] != '\n')
    fgets_or_exit(buf, 255, client);
}

void send_stats(FILE *client)
{
  char path_to_exe[64];
  sprintf(path_to_exe, "/proc/%d/exe", getpid());
  char resolved_path_to_exe[1024];
  realpath(path_to_exe, resolved_path_to_exe);


  int fd_stats;
  int sread;
  char buffer_reader[256];
  sprintf(resolved_path_to_exe, "%s/stats.html", dirname(resolved_path_to_exe));
  printf("%s\n", resolved_path_to_exe);
  if((fd_stats = open(resolved_path_to_exe, O_RDONLY)) > -1)
  {
    send_status(client, 200, "OK");
    fprintf(client, "Content-Length: %d\r\n\r\n", get_file_size(fd_stats));
    while((sread = read(fd_stats, buffer_reader, 256)) > 0)
    {
      fprintf(client, "%s", buffer_reader);
    }
    if(sread == -1)
    {
      perror("read");
    }
  }
  else
  {
    send_response(client, 404, "Not Found", "Not Found\r\n");
  }  
}

void send_status(FILE *client, int code, const char *reason_phrase)
{
  fprintf(client, "HTTP/1.1 %d %s\r\n", code, reason_phrase);
  web_stats *stats = get_stats();
  if(code == 200)
    ++stats->ok_200;
  else if(code == 400)
    ++stats->ko_400;
  else if(code == 403)
    ++stats->ko_403;
  else if(code == 404)
    ++stats->ko_404;
}

void send_response(FILE *client, int code, const char *reason_phrase, const char *message_body)
{
  send_status(client, code, reason_phrase);
  fprintf(client, "Content-Length: %zu\r\n\r\n", strlen(message_body));

  fprintf(client, "%s", message_body);
}

char *rewrite_url(char *url)
{
  char *absolute_path;
  char *buf = strdup(url);

  if((absolute_path = strtok(buf, "?")) == NULL)
    return NULL;
  return (strcmp(absolute_path, "/") == 0)?"index.html":++absolute_path;
}

int check_and_open(const char *url, const char *document_root)
{
  char url_cp[strlen(url)];
  int len = strlen(document_root);
  char doc_cp[len];

  strcpy(url_cp, url);
  strcpy(doc_cp, document_root);
                    
  char *real_path;
  if((real_path = realpath(strcat(doc_cp, url_cp), NULL)) == NULL)
    return -404;
  
  printf("%s\n", real_path);
  if(strncmp(document_root, real_path, len) != 0)
    return -403;
  
  int fd;
  if((fd = open(real_path, O_RDONLY)) == -1)
  {
    perror("open");
    return -1;
  }
  return fd;
}

int get_file_size(int fd)
{
  struct stat stat_ressource;
  if(fstat(fd, &stat_ressource))
  {
    perror("fstat");
    return -1;
  }
  return stat_ressource.st_size;
}

int copy(int in, int out)
{
  char buf[1024];
  int sread;
  
  while((sread = read(in, buf, 1024)) > 0)
  {
    if(write(out, buf, sread) == -1)
    {
      perror("write");
      return -1;
    }
  }
  if(sread == -1)
  {
    perror("read");
    return -1;
  }
  return 0;
}


void traitement_signal(int sig)
{
  printf("Signal %d reçu\n",sig);
  /* attend un fils */
  int status;
  if(sig == SIGCHLD)
  {
    if(waitpid(-1, &status, WUNTRACED) == -1)
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
