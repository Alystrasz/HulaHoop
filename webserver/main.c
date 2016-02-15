#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>

#include "socket.h"

#define PORT 8080
#define BUFFER_READER 1500

extern int socket_serveur;
int main(void){



  //Création du serveur sur le port PORT
  if(creer_serveur(PORT) == -1)
  {
    perror("Creation du serveur");
    //TODO gestion des erreurs
    return 1;
  }

  initialiser_signaux();

  
  const char* message_bienvenue = "                                                                                             \n                                                                                             \n                                                                                             \n                                                                                             \n                                                                                             \n                                                                                             \n                                 .::.`             .``                                       \n                               `::,.``           ...``                                       \n                               :;;,.```        ..,,..`                                       \n                              ,;::,.``` `    ...,:,:.`                                       \n                              :;::,,.```:  .,..,,:;,.`                                       \n                             `:,;:,,,,``,`..,,,,,,:,``                                       \n                             `:,;:,,.:,:.``,,,,.``:.``                                       \n                              ,,':,,;;`,:``,.`   `,.``                                       \n                              .,';,,,.`,,```     .,```                                       \n                              `,;';,,,.`,``      ,.```                                       \n                              `,:::,,,..:,`      ,```                                        \n                               :,:;,,,,...`     ..```                                        \n                              `:';,:,,,,,,`    `,.```                                        \n                              ,::'+;:,,,.      :;.``                                         \n                             .,:::';,:,,``    ,;;:``                                         \n                           `..,.,::;:,,.``   `;:;,``                                         \n                      `````...```.``::.`.`   ::,:,,`                                         \n                  ``.`````....,.```,::,`,```::,:,:,,                                         \n               .....`.......,` ';  `,,:`,````;;,::,                                          \n             .........,,,.`    ;+  ,,,,```````::..,                                          \n            `,.....,,,,        :; `,.,..``````,:..`                                          \n             .,,..``````` `,:,,,;,,`....`````..,,.,                                          \n              `..,.````````,::::::;:`..`````.`,,,.`                                          \n                `........`.,:;::.:::,,`````.:,,,,..                                          \n                  `,,,.....:::;:,::::,,,,,,,,,,,,,.`                                         \n                    ``.....::;;::::::::,,,,,,,,:,,,.                                         \n                          `,:;;:;.:::::::,:::::::,,.                                         \n                             `.,,,::::::::::::::::,.                                         \n                               `;;,:;:::::::::::::,.                                         \n                               .:,,::::::::::::,,,..                                         \n                               `: ,.;:::::::,:::,,..                                         \n                               `:  ::::::::::,,,,...                                         \n                                .  ::;:;::,:,,,,....`                                        \n                               `.   '::::,::,,,,,....                                        \n                               `.   :;,:::::,,,,,,...                                        \n                               .,    +,:::::,,,,,,,.`                                        \n                               .:    :;,::::,,,,...`                                         \n                               .:    `+::::::,,..```                                         \n                               ,:     .:::::,```````                                         \n                               ,:     ;.,,..````````                                         \n        ` ```.,..,:,,,,:,,:,````.```   .....````````                                         \n      .:,,:,:;.,:,:`::,,::..``.`  .`` `.,...````````  `                                      \n   `` `.,;`::`.. `             `` `` `.......``````````.,::,.`.`                             \n  `.`.,.`                            .........`````  ` ,,,::,, ,,::::,..`                    \n  ..`,                              +...,.....`````           ````:;::`:,::::,`.             \n  ..```                             ++;,...`..`````                      `:;:.,,`,``         \n   ..``.`                          ;`..+'......````,                          .`.:`..```     \n    .`;;::`                       :'+` ,:'+,...```,,                               `.`..``   \n     `:;;;;:,.                    ;''+'` `;,;#+'+'.`                                   ,,``` \n        `::::::,,.`              .'''++++;`  `.:' :                                     ,,,, \n          ``,:::::::,,`          :''''''''+'';;'`::                                    .,::: \n              ..,::,:,:::,,.     :'''''';;;+''';,:`                                 `,::::;` \n                  `,:::,,,,,,,,,,,'''::;;;;'';';;,                            .,,,::::::;:   \n                        .:,,,,,,....      `   `,:.       ``    ```   `````.:::::::::::,.     \n                              .,,,,`                                  `` ``:,::::,` `        \n                                 ,;'';:;;:.`   `                     ``````.`                \n                                 .'''''''':''+++.:                                           \n                                  '+'''';;:'++'',`                                           \n                                  '+'''';;:;'''',                                            \n                                  ''''''';;:'+'':                                            \n                                  ;'''''';;:;+;',                                            \n                                  ''+''''';;,'''`                                            \n                                  .''+'''';;,'';                                             \n                                   '+'+'''';:'';                                             \n                                   #'''''''';'';.                                            \n                                   +'++''''';:'`                                             \n                                   ,'++++''';:' '                                            \n                                    @''++'''':: .                                            \n                                    #'+'+'''';`                                              \n                                    '#++'+''''`                                              \n                                    ,#'++'''''.                                              \n                                    `#++++'''':                                              \n                                     '#++'+''';                                              \n                                     +#+++'+'''                                              \n                                      +#+++''''.                                             \n                                      +'#++++''.                                             \n                                      .'+++++''`                                             \n                                       #''+++++                                              \n                                       '#+'+++'                                              \n                                        '@+'++                                               \n                                                                                             \n                                                                                             \n\n";

  /* Acceptation d'une connexion */
  int socket_client;
  char buffer_reader[BUFFER_READER];
  int sread = 0;
 
  while(1)
  {
    if((socket_client = accept(socket_serveur, NULL, NULL)) == -1)
    {
      perror("accept");
      //TODO gestion des erreurs
      return 1;
    }
  
    if(!fork())
    {
      initialiser_signaux();

    
      /* écrit le message de bienvenue */
      if(write(socket_client, message_bienvenue, strlen(message_bienvenue)) < 0)
      {
	perror("write");
	return 1;
      }
     
      /* Service de bégayement */
      while((sread = read(socket_client, buffer_reader, BUFFER_READER)) > 0)
      {
	if(write(socket_client, buffer_reader, sread) < 0)
	{ 
	  perror("write");
	  return 1;
	}  
      }
      if(sread == -1)
      {
	perror("read");
	return 1;
      }
      //TODO le père doit traité erreur fils
    }
    else
    {
      wait(NULL);
    }
    close(socket_client);
  }
  close(socket_serveur);
  return 0;
}
