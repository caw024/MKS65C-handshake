#include "pipe_networking.h"


/* static void sighandler2(int signo){ */
/*   if (signo == SIGINT){ */
/*     printf("Exiting due to SIGINT\n"); */
/*     exit(0); */
/*   } */
/* } */

/*=========================%
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {

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
  printf("%s\n", buff);

  //removes WKP
  remove("WKP");

  //5. server connects to client's fifo and sends msg  
  char *send = "I got you";
  int writept = open(buff, O_WRONLY);
  write(writept, send, BUFFER_SIZE);
  close(writept);

  char buf[100];

  read(readpt, buf, 100);
  printf("%s\n", buf);
  close(readpt);

       
  //signal(SIGINT, sighandler2);
  *to_client = readpt;
  
  return writept; //write
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

  //while(1){

  
  //3. client connect toserver and sends private fifo
  char *send1 = "pserver";
  write(writept, send1, 100);
  printf("client sent private fifo to server\n");

  //scanf();
  
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

  close(readpt);
  //readpt is still the same
  
  //remove private fifo
  remove("pserver");

  char *send = "take that sucker";
  write(writept, send, 100);
  printf("client sent msg to server\n");
  
  *to_server = writept;
  //}
  return readpt; //read
}
