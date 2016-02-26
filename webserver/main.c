#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>

#include "socket.h"

#define PORT 8080
#define BUFFER_READER 255

extern int socket_serveur;

int main(void){



  //Création du serveur sur le port PORT
  if(creer_serveur(PORT) == 1)
  {
    perror("Creation du serveur");
    return EXIT_FAILURE;
  }

  initialiser_signaux();

  const char* msg200 = "HTTP/1.1 200 OK\r\nContent-Length: ";
  const char* msg400 = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 17\r\n\r\n400 Bad request\r\n";
  //const char* msg404 = "HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-Length: 15\r\n\r\n404 Not Found\r\n";
  const char* msg_bienvenue = "                                                                                             \n                                                                                             \n                                                                                             \n                                                                                             \n                                                                                             \n                                                                                             \n                                 .::.`             .``                                       \n                               `::,.``           ...``                                       \n                               :;;,.```        ..,,..`                                       \n                              ,;::,.``` `    ...,:,:.`                                       \n                              :;::,,.```:  .,..,,:;,.`                                       \n                             `:,;:,,,,``,`..,,,,,,:,``                                       \n                             `:,;:,,.:,:.``,,,,.``:.``                                       \n                              ,,':,,;;`,:``,.`   `,.``                                       \n                              .,';,,,.`,,```     .,```                                       \n                              `,;';,,,.`,``      ,.```                                       \n                              `,:::,,,..:,`      ,```                                        \n                               :,:;,,,,...`     ..```                                        \n                              `:';,:,,,,,,`    `,.```                                        \n                              ,::'+;:,,,.      :;.``                                         \n                             .,:::';,:,,``    ,;;:``                                         \n                           `..,.,::;:,,.``   `;:;,``                                         \n                      `````...```.``::.`.`   ::,:,,`                                         \n                  ``.`````....,.```,::,`,```::,:,:,,                                         \n               .....`.......,` ';  `,,:`,````;;,::,                                          \n             .........,,,.`    ;+  ,,,,```````::..,                                          \n            `,.....,,,,        :; `,.,..``````,:..`                                          \n             .,,..``````` `,:,,,;,,`....`````..,,.,                                          \n              `..,.````````,::::::;:`..`````.`,,,.`                                          \n                `........`.,:;::.:::,,`````.:,,,,..                                          \n                  `,,,.....:::;:,::::,,,,,,,,,,,,,.`                                         \n                    ``.....::;;::::::::,,,,,,,,:,,,.                                         \n                          `,:;;:;.:::::::,:::::::,,.                                         \n                             `.,,,::::::::::::::::,.                                         \n                               `;;,:;:::::::::::::,.                                         \n                               .:,,::::::::::::,,,..                                         \n                               `: ,.;:::::::,:::,,..                                         \n                               `:  ::::::::::,,,,...                                         \n                                .  ::;:;::,:,,,,....`                                        \n                               `.   '::::,::,,,,,....                                        \n                               `.   :;,:::::,,,,,,...                                        \n                               .,    +,:::::,,,,,,,.`                                        \n                               .:    :;,::::,,,,...`                                         \n                               .:    `+::::::,,..```                                         \n                               ,:     .:::::,```````                                         \n                               ,:     ;.,,..````````                                         \n        ` ```.,..,:,,,,:,,:,````.```   .....````````                                         \n      .:,,:,:;.,:,:`::,,::..``.`  .`` `.,...````````  `                                      \n   `` `.,;`::`.. `             `` `` `.......``````````.,::,.`.`                             \n  `.`.,.`                            .........`````  ` ,,,::,, ,,::::,..`                    \n  ..`,                              +...,.....`````           ````:;::`:,::::,`.             \n  ..```                             ++;,...`..`````                      `:;:.,,`,``         \n   ..``.`                          ;`..+'......````,                          .`.:`..```     \n    .`;;::`                       :'+` ,:'+,...```,,                               `.`..``   \n     `:;;;;:,.                    ;''+'` `;,;#+'+'.`                                   ,,``` \n        `::::::,,.`              .'''++++;`  `.:' :                                     ,,,, \n          ``,:::::::,,`          :''''''''+'';;'`::                                    .,::: \n              ..,::,:,:::,,.     :'''''';;;+''';,:`                                 `,::::;` \n                  `,:::,,,,,,,,,,,'''::;;;;'';';;,                            .,,,::::::;:   \n                        .:,,,,,,....      `   `,:.       ``    ```   `````.:::::::::::,.     \n                              .,,,,`                                  `` ``:,::::,` `        \n                                 ,;'';:;;:.`   `                     ``````.`                \n                                 .'''''''':''+++.:                                           \n                                  '+'''';;:'++'',`                                           \n                                  '+'''';;:;'''',                                            \n                                  ''''''';;:'+'':                                            \n                                  ;'''''';;:;+;',                                            \n                                  ''+''''';;,'''`                                            \n                                  .''+'''';;,'';                                             \n                                   '+'+'''';:'';                                             \n                                   #'''''''';'';.                                            \n                                   +'++''''';:'`                                             \n                                   ,'++++''';:' '                                            \n                                    @''++'''':: .                                            \n                                    #'+'+'''';`                                              \n                                    '#++'+''''`                                              \n                                    ,#'++'''''.                                              \n                                    `#++++'''':                                              \n                                     '#++'+''';                                              \n                                     +#+++'+'''                                              \n                                      +#+++''''.                                             \n                                      +'#++++''.                                             \n                                      .'+++++''`                                             \n                                       #''+++++                                              \n                                       '#+'+++'                                              \n                                        '@+'++                                               \n                                                                                             \n                                                                                             \n\n";

  /* Acceptation d'une connexion */
  int socket_client;
  char buffer_reader[BUFFER_READER];
  FILE *flux_client;
 
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
      /* écrit le message de bienvenue */
     
      /* Service de bégayement 
      while(fgets(buffer_reader, BUFFER_READER, flux_client) != NULL)
      {
	fprintf(stdout, "<HulaHoop>");
	fprintf(stdout, "%s", buffer_reader);
      }
      */
      
      fgets(buffer_reader, BUFFER_READER, flux_client);
      if(simple_get(buffer_reader))
      {
	while(buffer_reader[0] != '\r' && buffer_reader[1] != '\n')
	  fgets(buffer_reader, BUFFER_READER, flux_client);
	
	fprintf(flux_client, msg200, strlen(msg200));
	fprintf(flux_client, "%zu\r\n\r\n", strlen(msg_bienvenue));
	fprintf(flux_client, msg_bienvenue, strlen(msg_bienvenue));

	//fflush(stdout);
      }
      else
      {
	fprintf(flux_client, msg400, strlen(msg400));
      }
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
