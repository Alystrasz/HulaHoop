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

  
  const char* message_bienvenue = "                                                                                             \n                                                                                             \n                                                                                             \n                                                                                             \n                                                                                             \n                                                                                             \n                                 .::.`             .``                                       \n                               `::,.``           ...``                                       \n                               :;;,.```        ..,,..`                                       \n                              ,;::,.``` `    ...,:,:.`                                       \n                              :;::,,.```:  .,..,,:;,.`                                       \n                             `:,;:,,,,``,`..,,,,,,:,``                                       \n                             `:,;:,,.:,:.``,,,,.``:.``                                       \n                              ,,':,,;;`,:``,.`   `,.``                                       \n                              .,';,,,.`,,```     .,```                                       \n                              `,;';,,,.`,``      ,.```                                       \n                              `,:::,,,..:,`      ,```                                        \n                               :,:;,,,,...`     ..```                                        \n                              `:';,:,,,,,,`    `,.```                                        \n                              ,::'+;:,,,.      :;.``                                         \n                             .,:::';,:,,``    ,;;:``                                         \n                           `..,.,::;:,,.``   `;:;,``                                         \n                      `````...```.``::.`.`   ::,:,,`                                         \n                  ``.`````....,.```,::,`,```::,:,:,,                                         \n               .....`.......,` ';  `,,:`,````;;,::,                                          \n             .........,,,.`    ;+  ,,,,```````::..,                                          \n            `,.....,,,,        :; `,.,..``````,:..`                                          \n             .,,..``````` `,:,,,;,,`....`````..,,.,                                          \n              `..,.````````,::::::;:`..`````.`,,,.`                                          \n                `........`.,:;::.:::,,`````.:,,,,..                                          \n                  `,,,.....:::;:,::::,,,,,,,,,,,,,.`                                         \n                    ``.....::;;::::::::,,,,,,,,:,,,.                                         \n                          `,:;;:;.:::::::,:::::::,,.                                         \n                             `.,,,::::::::::::::::,.                                         \n                               `;;,:;:::::::::::::,.                                         \n                               .:,,::::::::::::,,,..                                         \n                               `: ,.;:::::::,:::,,..                                         \n                               `:  ::::::::::,,,,...                                         \n                                .  ::;:;::,:,,,,....`                                        \n                               `.   '::::,::,,,,,....                                        \n                               `.   :;,:::::,,,,,,...                                        \n                               .,    +,:::::,,,,,,,.`                                        \n                               .:    :;,::::,,,,...`                                         \n                               .:    `+::::::,,..```                                         \n                               ,:     .:::::,```````                                         \n                               ,:     ;.,,..````````                                         \n        ` ```.,..,:,,,,:,,:,````.```   .....````````                                         \n      .:,,:,:;.,:,:`::,,::..``.`  .`` `.,...````````  `                                      \n   `` `.,;`::`.. `             `` `` `.......``````````.,::,.`.`                             \n  `.`.,.`                            .........`````  ` ,,,::,, ,,::::,..`                    \n  ..`,                              +...,.....`````           ````:;::`:,::::,`.             \n  ..```                             ++;,...`..`````                      `:;:.,,`,``         \n   ..``.`                          ;`..+'......````,                          .`.:`..```     \n    .`;;::`                       :'+` ,:'+,...```,,                               `.`..``   \n     `:;;;;:,.                    ;''+'` `;,;#+'+'.`                                   ,,``` \n        `::::::,,.`              .'''++++;`  `.:' :                                     ,,,, \n          ``,:::::::,,`          :''''''''+'';;'`::                                    .,::: \n              ..,::,:,:::,,.     :'''''';;;+''';,:`                                 `,::::;` \n                  `,:::,,,,,,,,,,,'''::;;;;'';';;,                            .,,,::::::;:   \n                        .:,,,,,,....      `   `,:.       ``    ```   `````.:::::::::::,.     \n                              .,,,,`                                  `` ``:,::::,` `        \n                                 ,;'';:;;:.`   `                     ``````.`                \n                                 .'''''''':''+++.:                                           \n                                  '+'''';;:'++'',`                                           \n                                  '+'''';;:;'''',                                            \n                                  ''''''';;:'+'':                                            \n                                  ;'''''';;:;+;',                                            \n                                  ''+''''';;,'''`                                            \n                                  .''+'''';;,'';                                             \n                                   '+'+'''';:'';                                             \n                                   #'''''''';'';.                                            \n                                   +'++''''';:'`                                             \n                                   ,'++++''';:' '                                            \n                                    @''++'''':: .                                            \n                                    #'+'+'''';`                                              \n                                    '#++'+''''`                                              \n                                    ,#'++'''''.                                              \n                                    `#++++'''':                                              \n                                     '#++'+''';                                              \n                                     +#+++'+'''                                              \n                                      +#+++''''.                                             \n                                      +'#++++''.                                             \n                                      .'+++++''`                                             \n                                       #''+++++                                              \n                                       '#+'+++'                                              \n                                        '@+'++                                               \n                                                                                             \n                                                                                             \n\n";

  /* Acceptation d'une connexion */
  int socket_client;
  char buffer_reader[BUFFER_READER];
  FILE *flux_client;
 
  while(1)
  {
    /* accepte une connexion  */
    if((socket_client = accept(socket_serveur, NULL, NULL)) == -1)
    {
      perror("accept");
      return EXIT_FAILURE;
    }

    /* ouvre un flux */
    if((flux_client = fdopen(socket_client, "w+")) == NULL)
    {
      perror("fdopen");
      return EXIT_FAILURE;
    }
  
    if(!fork())
    {
      /* écrit le message de bienvenue */
      fprintf(flux_client, message_bienvenue, strlen(message_bienvenue));
     
      /* Service de bégayement */
      while(fgets(buffer_reader, BUFFER_READER, flux_client) != NULL)
      {
	fprintf(flux_client, "<HulaHoop>");
	fprintf(flux_client, "%s", buffer_reader);
      }
    }
    else
    {
      wait(NULL);
    }
    if(fclose(flux_client) == -1)
    {
      perror("fclose");
      return EXIT_FAILURE;
    }
    if(close(socket_client) == -1)
    {
      perror("close");
      return EXIT_FAILURE;
    }
  }
  if(close(socket_serveur) == -1)
  {
    perror("close");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
