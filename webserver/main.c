#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>



#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>

#include <unistd.h>

#include "socket.h"
#include "stats.h"

#define PORT 8080
#define BUFFER_READER 255

extern int socket_serveur;


void usage(void)
{
  fprintf(stderr, "HulaHoop dossier_accessible\n");
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv){
  if(argc != 2)
    usage();
  
  const char *document_root = *(++argv);
  struct stat stat_root;
  if(stat(document_root, &stat_root) == -1)
  {
    perror("stat");
    return EXIT_FAILURE;
  }

  if(!S_ISDIR(stat_root.st_mode))
    usage();
  if(access(document_root, X_OK) == -1)
    usage();

  printf("%s\n", document_root);

  //Création du serveur sur le port PORT
  if(creer_serveur(PORT) == 1)
  {
    perror("Creation du serveur");
    return EXIT_FAILURE;
  }

  initialiser_signaux();

  const char* msg_bienvenue = "                                                                                             \n                                                                                             \n                                                                                             \n                                                                                             \n                                                                                             \n                                                                                             \n                                 .::.`             .``                                       \n                               `::,.``           ...``                                       \n                               :;;,.```        ..,,..`                                       \n                              ,;::,.``` `    ...,:,:.`                                       \n                              :;::,,.```:  .,..,,:;,.`                                       \n                             `:,;:,,,,``,`..,,,,,,:,``                                       \n                             `:,;:,,.:,:.``,,,,.``:.``                                       \n                              ,,':,,;;`,:``,.`   `,.``                                       \n                              .,';,,,.`,,```     .,```                                       \n                              `,;';,,,.`,``      ,.```                                       \n                              `,:::,,,..:,`      ,```                                        \n                               :,:;,,,,...`     ..```                                        \n                              `:';,:,,,,,,`    `,.```                                        \n                              ,::'+;:,,,.      :;.``                                         \n                             .,:::';,:,,``    ,;;:``                                         \n                           `..,.,::;:,,.``   `;:;,``                                         \n                      `````...```.``::.`.`   ::,:,,`                                         \n                  ``.`````....,.```,::,`,```::,:,:,,                                         \n               .....`.......,` ';  `,,:`,````;;,::,                                          \n             .........,,,.`    ;+  ,,,,```````::..,                                          \n            `,.....,,,,        :; `,.,..``````,:..`                                          \n             .,,..``````` `,:,,,;,,`....`````..,,.,                                          \n              `..,.````````,::::::;:`..`````.`,,,.`                                          \n                `........`.,:;::.:::,,`````.:,,,,..                                          \n                  `,,,.....:::;:,::::,,,,,,,,,,,,,.`                                         \n                    ``.....::;;::::::::,,,,,,,,:,,,.                                         \n                          `,:;;:;.:::::::,:::::::,,.                                         \n                             `.,,,::::::::::::::::,.                                         \n                               `;;,:;:::::::::::::,.                                         \n                               .:,,::::::::::::,,,..                                         \n                               `: ,.;:::::::,:::,,..                                         \n                               `:  ::::::::::,,,,...                                         \n                                .  ::;:;::,:,,,,....`                                        \n                               `.   '::::,::,,,,,....                                        \n                               `.   :;,:::::,,,,,,...                                        \n                               .,    +,:::::,,,,,,,.`                                        \n                               .:    :;,::::,,,,...`                                         \n                               .:    `+::::::,,..```                                         \n                               ,:     .:::::,```````                                         \n                               ,:     ;.,,..````````                                         \n        ` ```.,..,:,,,,:,,:,````.```   .....````````                                         \n      .:,,:,:;.,:,:`::,,::..``.`  .`` `.,...````````  `                                      \n   `` `.,;`::`.. `             `` `` `.......``````````.,::,.`.`                             \n  `.`.,.`                            .........`````  ` ,,,::,, ,,::::,..`                    \n  ..`,                              +...,.....`````           ````:;::`:,::::,`.             \n  ..```                             ++;,...`..`````                      `:;:.,,`,``         \n   ..``.`                          ;`..+'......````,                          .`.:`..```     \n    .`;;::`                       :'+` ,:'+,...```,,                               `.`..``   \n     `:;;;;:,.                    ;''+'` `;,;#+'+'.`                                   ,,``` \n        `::::::,,.`              .'''++++;`  `.:' :                                     ,,,, \n          ``,:::::::,,`          :''''''''+'';;'`::                                    .,::: \n              ..,::,:,:::,,.     :'''''';;;+''';,:`                                 `,::::;` \n                  `,:::,,,,,,,,,,,'''::;;;;'';';;,                            .,,,::::::;:   \n                        .:,,,,,,....      `   `,:.       ``    ```   `````.:::::::::::,.     \n                              .,,,,`                                  `` ``:,::::,` `        \n                                 ,;'';:;;:.`   `                     ``````.`                \n                                 .'''''''':''+++.:                                           \n                                  '+'''';;:'++'',`                                           \n                                  '+'''';;:;'''',                                            \n                                  ''''''';;:'+'':                                            \n                                  ;'''''';;:;+;',                                            \n                                  ''+''''';;,'''`                                            \n                                  .''+'''';;,'';                                             \n                                   '+'+'''';:'';                                             \n                                   #'''''''';'';.                                            \n                                   +'++''''';:'`                                             \n                                   ,'++++''';:' '                                            \n                                    @''++'''':: .                                            \n                                    #'+'+'''';`                                              \n                                    '#++'+''''`                                              \n                                    ,#'++'''''.                                              \n                                    `#++++'''':                                              \n                                     '#++'+''';                                              \n                                     +#+++'+'''                                              \n                                      +#+++''''.                                             \n                                      +'#++++''.                                             \n                                      .'+++++''`                                             \n                                       #''+++++                                              \n                                       '#+'+++'                                              \n                                        '@+'++                                               \n                                                                                             \n                                                                                             \n\n";

  int socket_client;
  int request_client;
  int ressource;
  char buffer_reader[BUFFER_READER];
  FILE *flux_client;
  http_request http_req;
  web_stats *stats = get_stats();

  printf("%s\n",msg_bienvenue);

  while(1)
  {
    /* accepte une connexion  */
    if((socket_client = accept(socket_serveur, NULL, NULL)) == -1)
      perror("accept");

    ++stats->served_connections;
    
    /* ouvre un flux */
    if((flux_client = fdopen(socket_client, "w+")) == NULL)
    {
      close(socket_serveur);
      close(socket_client);
      perror("fdopen");
      return EXIT_FAILURE;
    }
  
    if(!fork())
    {
      /* on recupere ce que dis le client  */
      fgets_or_exit(buffer_reader, BUFFER_READER, flux_client);
      
      /* on regarde si on comprend ce qu'il nous dis  */
      request_client = parse_http_request(buffer_reader, &http_req);
      skip_headers(flux_client);

      ++stats->served_requests;
      
      /* on repond au client  */
      if(request_client == 0)
	send_response(flux_client, 400, "Bad Request", "Bad request\r\n");
      else if(http_req.method == HTTP_UNSUPPORTED)
	send_response(flux_client, 405, "Method Not Allowed" , "Method Not Allowed\r\n");
      else if(strcmp(http_req.url, "/stats") == 0)
	send_stats(flux_client);
      else if((ressource = check_and_open(rewrite_url(http_req.url), document_root)) > 0)
      {
	send_status(flux_client, 200, "OK");
	//fonctionne mieux sans 
	//fprintf(flux_client, "Content-Type: text/html\r\n");
	fprintf(flux_client, "Content-Length: %d\r\n\r\n", get_file_size(ressource));
	fflush(flux_client);

	copy(ressource, socket_client);
      }
      else if(ressource == -403)
	send_response(flux_client, 403, "Access Denied", "Access Denied\r\n");
      else
	send_response(flux_client, 404, "Not Found", "Not Found\r\n");

      printf("served_connections: %d\nserved_requests: %d\nok_200: %d\nko_400: %d\nko_403: %d\nko_404: %d\n", stats->served_connections, stats->served_requests, stats->ok_200, stats->ko_400, stats->ko_403, stats->ko_404);
      
      if(fclose(flux_client) == -1)
	perror("fclose");
    }
    else
    {
      close(socket_client);
      //wait(NULL);
    }
  }
  close(socket_serveur);
  return EXIT_SUCCESS;
}
