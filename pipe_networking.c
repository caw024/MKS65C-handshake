#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  //1. server created WKP
  int fdfifo = mkfifo("WKP", 0777);
  if (fdfifo == -1){
    printf("Error: %s\n", strerror(errno));
    exit(1);
  }

  //client_handshake(&fdfifo);
  //server waits for response
  while (*to_client = open("WKP", O_RDONLY) < 0);

  printf("to_client: %d\n", *to_client);

  
  //4. servers receives client's message
  char *buff = malloc(100);
  printf("alright\n");
  if (read(*to_client, buff, 100) < 0){
    printf("Error: %s\n", strerror(errno));
    exit(1);
  }
  printf("%s\n", buff);
  printf("lmao\n");

  //removes WKP
  close(fdfifo);

  //5. server connects to client's fifo and sends msg
  char *send = "To client, I received your message. From sender.";
  write(*to_client, send ,BUFFER_SIZE);


  return 0;
}


/*=========================
  client_handshake
  args: int * to_server (well known)

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  int fd;
  //2. client creates private server
  int fdfifo = mkfifo("pserver", 0644);
  if (fdfifo == -1){
    printf("Error: %s", strerror(errno));
    exit(1);
  }

  *to_server = open("WKP", O_WRONLY);
  printf("to_server: %d\n", *to_server);
 
  
  //3. client connect toserver and sends private fifo
  char *send1 = "To server, I want to connect. From client.";
  write(*to_server, send1, 100);
  printf("writing\n");

  //server_handshake(&fdfifo);
  
  //client waits for response (err)
  while(*to_server = open("pserver", O_RDONLY) < 0);

  //6. client receives server's msg
  char *buff = malloc(BUFFER_SIZE);
  if (read(*to_server, buff, 100) < 0){
    printf("Error: %s\n", strerror(errno));
    exit(1);
  }  printf("%s\n", buff);


  //7. client sends response to server
  char *send = "To server, I received your message. From client.";
  write(*to_server, send ,BUFFER_SIZE);

  //remove private fifo
  close(fdfifo);

  
  return 0;
}
