#include "pipe_networking.h"
//SERVER


static void sighandler(int signo){  
  if (signo == SIGINT){
    printf("Exiting due to SIGINT\n");  
    if (remove("WKP") == -1){ 
      printf("tried to remove WKP: %s\n", strerror(errno)); 
      exit(1); 
    }
    printf("successfully removed WKP\n");
    exit(0);
  }  
}

int main() {

  int to_client;
  int from_client;

  while(1){
    //Does handshake
    from_client = server_handshake( &to_client );

    //read from client
    char exclam[3] = "ha";
    char lol[100];

    signal(SIGINT, sighandler);

    while(read(from_client, lol, 100) > 0){
      //do stuff

      printf("given value: %s\n", lol);

      //close(readpt);

      strcat(lol,exclam);
      printf("new argument: %s\n\n", lol);

      //write side
      //write to client

      // sleep(1);
      if (write(to_client, lol, 100) < 0){
	printf("error: %s\n", strerror(errno));
      }
    }
    
    printf("repeat\n");
  }
}
  

