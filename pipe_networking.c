#include "pipe_networking.h"


/*=========================%
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  int fd;
  //1. server created WKP
  int fdfifo = mkfifo("WKP", 0777);
  if (fdfifo == -1){
    printf("Error: %s\n", strerror(errno));
    exit(1);
  }

  //client_handshake(&fdfifo);
  //server waits for response
  fd = open("WKP", O_RDONLY);

  printf("to_client: %d\n", *to_client);

  
  //4. servers receives client's message
  char buff[50];
  printf("alright\n");
  int k = read(fd, buff, 51*sizeof(char));
  
  printf("value of k: %d\n", k);
  if (k < 0){
    printf("Error: %s\n", strerror(errno));
    exit(1);
  }
  printf("%s\n", buff);

  //removes WKP
  close(fdfifo);
  close(fd);

  //5. server connects to client's fifo and sends msg
  printf("sending message\n");
  
  char *send = "To client, I received your message. From sender.";
  fd = open(buff, O_WRONLY);
  write(fd, send ,BUFFER_SIZE);


  
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

  fd = open("WKP", O_WRONLY);
  printf("to_server: %d\n", *to_server);
 
  
  //3. client connect toserver and sends private fifo
  char *send1 = "pserver";
  write(fd, send1, 50);
  printf("writing\n");

  close(fd);
  //server_handshake(&fdfifo);
  
  //client waits for response (err)
  fd = open("pserver", O_RDONLY);

  //6. client receives server's msg
  char *buff = malloc(BUFFER_SIZE);
  if (read(fd, buff, 100) < 0){
    printf("Error: %s\n", strerror(errno));
    exit(1);
  }
  printf("%s\n", buff);


  //7. client sends response to server
  fd = open(buff, O_WRONLY);

  char *send = "To server, I received your message. From client.";
  write(fd, send ,BUFFER_SIZE);

  //remove private fifo
  close(fdfifo);

  
  return 0;
}
