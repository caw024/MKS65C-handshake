#include "pipe_networking.h"
//CLIENT

int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );

  while(1){
    char *input = malloc(100);
    printf("input: ");
    scanf("%s", input);

    //fgets(input, 100, stdin);
    //write side -> read on server
    if (write(to_server, input, 100) < 0){
      printf("error: %s\n", strerror(errno));
      exit(1);
    }
    free(input);

    char *buf = malloc(100);
    //read side
    //PROBLEM

    //waits for server to send smth
    if (read(from_server, buf, 100) < 0){
      printf("error: %s\n", strerror(errno));
      exit(1);
    }

    //signal(SIGINT, sighandler2);


    printf("Message received: %s\n", buf);
    printf("\n");
    free(buf);
  }
}
