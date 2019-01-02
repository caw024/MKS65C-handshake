#include "pipe_networking.h"
//SERVER

int main() {

  int to_client;
  int from_client;

  while(1){
    //Does handshake
    from_client = server_handshake( &to_client );

    //read from client
    char exclam[3] = "ha";
    char lol[100];

    while(read(from_client, lol, 100) > 0){
      //do stuff


      printf("given value: %s\n", lol);
      //close(readpt);

      strcat(lol,exclam);
      printf("new argument: %s\n", lol);

      //write side
      //write to client
      //signal(SIGINT, sighandler2);
  
      // sleep(1);
      if (write(to_client, lol, 100) < 0){
	printf("error: %s\n", strerror(errno));
	exit(1);
      }

    }
    
    printf("the very end/n");
  
  }
}
  

