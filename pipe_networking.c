#include "pipe_networking.h"



/*=========================%
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  int writept;

    //1. server created WKP
    int fdfifo = mkfifo("WKP", 0644);
    if (fdfifo == -1){
      printf("Error: %s\n", strerror(errno));
      exit(1);
    }


    //server waits for response
    int readpt = open("WKP", O_RDONLY);

    printf("to_client: %d\n", *to_client);

  
    //4. servers receives client's message
    char buff[100];
    int k = read(readpt, buff, 100);
  
    if (k < 0){
      printf("Error: %s\n", strerror(errno));
      exit(1);
    }

    remove("WKP");

    printf("server received fifo from client: %s\n", buff);

    //removes WKP

    //5. server connects to client's fifo and sends msg  
    char *send = "I got you";
    writept = open(buff, O_WRONLY);
    write(writept, send, 100);
    //close(writept);

    char buf[100];
    read(readpt, buf, 100);
    printf("handshake completed: %s\n", buf);

 
    *to_client = writept;
    //handshake completed
    //close(readpt);

  
    return readpt; //write
}






/*=========================
  client_handshake
  args: int * to_server (well known)

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  printf("to_server: %d\n", *to_server);
  
  //2. client creates private server
  int fdfifo = mkfifo("pserver", 0644);
  if (fdfifo == -1){
    printf("Error: %s", strerror(errno));
    exit(1);
  }

  int writept = open("WKP", O_WRONLY);
  
  //3. client connect toserver and sends private fifo
  char *send1 = "pserver";
  write(writept, send1, 100);
  printf("client sent super private fifo to server\n");
  printf("send1: %s\n", send1);

  
  //client waits for response (err)
  int readpt = open("pserver", O_RDONLY);


  //6. client receives server's msg
  char buff[100];
  int k = read(readpt, buff, 100);
  if (k < 0){
    printf("Error: %s\n", strerror(errno));
    exit(1);
  }
  printf("client received this from server: %s\n", buff);

  //readpt is still the same
  

  char *send = "take that sucker";
  write(writept, send, 100);

  //remove private fifo
  remove("pserver"); 

  printf("client sent msg to server\n");
  
  *to_server = writept;
  
  return readpt; //read
}
