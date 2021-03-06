# ifndef __SOCKET_H__
# define __SOCKET_H__

enum http_method {
  HTTP_GET,
  HTTP_UNSUPPORTED,
};

typedef struct
{
  enum http_method method;
  int major_version;
  int minor_version;
  char *url;
} http_request;

int creer_serveur(int port);

char *fgets_or_exit(char *buffer, int size, FILE *stream);
int parse_http_request(const char *request_line, http_request *request);
void skip_headers(FILE *client);

void send_stats(FILE *client);
void send_status( FILE *client, int code, const char *reason_phrase);
void send_response(FILE *client, int code, const char *reason_phrase, const char *message_body);

char *rewrite_url(char *url);
int check_and_open(const char *url, const char *document_root);
int get_file_size(int fd);
int copy(int in, int out);

void traitement_signal(int sig);
void initialiser_signaux(void);
# endif
