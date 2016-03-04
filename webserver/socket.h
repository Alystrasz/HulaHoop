# ifndef __SOCKET_H__
# define __SOCKET_H__
/* * Crée une socket serveur qui écoute sur toute les interfaces IPv4
de la machine sur le port passé en paramètre . La socket retournée
doit pouvoir être utilisée directement par un appel à accept .
La fonction retourne -1 en cas d ’ erreur ou le descripteur de la
socket créée . */
int creer_serveur(int port);
char *fgets_or_exit(char *buffer, int size, FILE *stream);
int parse_http_request(const char *request_line, http_request *request);
void skip_headers(FILE *client);
void send_status( FILE *client, int code, const char *reason_phrase);
void send_response(FILE *client, int code, const char *reason_phrase, const char *message_body);
int simple_get(char *req);
void traitement_signal(int sig);
void initialiser_signaux(void);
# endif
