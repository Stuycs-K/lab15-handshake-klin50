#include "pipe_networking.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
//UPSTREAM = to the server / from the client
//DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  printf("SERVER SETUP\n");
  int from_client = 0;
  mkfifo("WKP",0666);
  from_client = open("WKP",O_RDONLY,0);
  remove("WKP");
  printf("FINISHED SETTING UP\n");
  return from_client;
}

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
  srand(getpid());
  char text[256];
  int rando[1];
  int from_client = server_setup();
  *to_client = open("PP",O_WRONLY,0);
  while(1){
    rando[0] = rand();
    write(*to_client,rando,sizeof(rando));
    sleep(1);
  }
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  char text[256];
  char buff[256];
  mkfifo("PP",0666);
  *to_server = open("WKP",O_WRONLY,0);
  int from_server = open("PP",O_RDONLY,0);
  remove("PP");
  int rando[1];
  while(1){
    if(read(from_server,rando,sizeof(rando)) == 0){
      break;
    }
    printf("%d\n",rando[0]);
  }
  return from_server;
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  int to_client  = open("PP",O_WRONLY,0);
  return to_client;
}
