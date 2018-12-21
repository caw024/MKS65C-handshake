#include "pipe_networking.h"


static void sighandler2(int signo){ 
  if (signo == SIGINT){ 
    printf("Exiting due to SIGINT\n"); 
    if (remove("WKP") == -1){
      printf("tried to remove WKP: %s\n", strerror(errno));
      exit(1);
    }
    exit(0);
  } 
} 

/*=========================%
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  int writept;

  while(1){
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

 
    *to_client = readpt;
    //handshake completed
    //close(readpt);

    char exclam[3] = "ha";
      char lol[100];


    while(1){
      //read from client
      if (read(readpt, lol, 100) < 0){
	printf("error: %s\n", strerror(errno));
	exit(1);
      }

      printf("given value: %s\n", lol);
      //close(readpt);

      strcat(lol,exclam);
      printf("new argument: %s\n", lol);

      //write side
      //write to client
      signal(SIGINT, sighandler2);
  
      printf("lmao: %d\n", writept);
      // sleep(1);
      if (write(writept, lol, 100) < 0){
	printf("error: %s\n", strerror(errno));
	exit(1);
      }
      printf("sent\n\n");

    }
    
    close(readpt);
    close(writept);
    printf("the very end/n");

  }
  
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
  remove("pserver"); 

  
  printf("client sent msg to server\n");
  
  //remove private fifo

  *to_server = writept;


  while(1){
    char *input = malloc(100);
    printf("input: ");
    scanf("%s", input);

    //fgets(input, 100, stdin);
    //write side -> read on server
    if (write(writept, input, 100) < 0){
      printf("error: %s\n", strerror(errno));
      exit(1);
    }
    free(input);

    char *buf = malloc(100);
    //read side
    //PROBLEM

    //waits for server to send smth
    if (read(readpt, buf, 100) < 0){
      printf("error: %s\n", strerror(errno));
      exit(1);
    }

    signal(SIGINT, sighandler2);


    printf("Message received: %s\n", buf);
    printf("\n");
    free(buf);
  }
  //signal(SIGINT, sighandler2);
  
  return readpt; //read
}
