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
  
  const char *document_root = rewrite_url(*(++argv));

  if(!S_IFDIR(document_root))
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
  char buffer_reader[BUFFER_READER];
  FILE *flux_client;
  http_request http_req;

  while(1)
  {
    /* accepte une connexion  */
    if((socket_client = accept(socket_serveur, NULL, NULL)) == -1)
      perror("accept");

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

      /* on repond au client  */
      if(request_client == 0)
	send_response(flux_client, 400, "Bad Request", "Bad request\r\n");
      else if(http_req.method == HTTP_UNSUPPORTED)
	send_response(flux_client, 405, "Method Not Allowed" , "Method Not Allowed\r\n");
      else if(strcmp(http_req.url, "/") == 0)
	send_response(flux_client, 200, "OK", msg_bienvenue);
      else
	send_response(flux_client, 404, "Not Found", "Not Found\r\n");
      
      if(fclose(flux_client) == -1)
	perror("fclose");
    }
    else
    {
      close(socket_client);
      wait(NULL);
    }
  }
  close(socket_serveur);
  return EXIT_SUCCESS;
}
