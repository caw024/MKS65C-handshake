#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  int fd;
  //1. server created WKP
  int fdfifo = mkfifo("server", 0644);
  if (fdfifo == -1){
    printf("Error: %s", strerror(errno));
    exit(1);
  }

  //server waits for response
  while (fd = open("server", O_RDONLY));

  //4. servers receives client's message
  char *buff = malloc(BUFFER_SIZE);
  read(fd, buff, BUFFER_SIZE);

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

  //3. client connect toserver and sends private fifo
  write(*to_server, , 100);

  //client waits for response (err)
  while(fd = open(*to_server, O_RDONLY));

  //6. client receives server's msg
  char *buff = malloc(BUFFER_SIZE);
  read(fd, buff, BUFFER_SIZE);

  //remove private fifo
  close(fdfifo);

  //7. client sends response to server
  char *send = "To server, I received your message. From client.";
  write(*to_client, send ,BUFFER_SIZE);
  
  return 0;
}
