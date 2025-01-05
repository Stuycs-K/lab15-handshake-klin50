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
  int from_client = 0;
  mkfifo("WKP",0666);
  from_client = open("WKP",O_RDONLY,0);
  remove("WKP");
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
  int from_client = server_setup();
  *to_client = open("PP",O_WRONLY,0);
  char buff[10];
  char syn_ack[] = "SYN_ACK";
  read(from_client,buff,sizeof(buff));
  if(strcmp(buff,"SYN") != 0){
    printf("Server handshake failed\n");
  }
  write(*to_client,syn_ack,sizeof(syn_ack));
  read(from_client,buff,sizeof(buff));
  if(strcmp(buff,"ACK") != 0){
    printf("Server handshake failed\n");
  }
  return from_client;
}

int server_handshake_half(int *to_client, int from_client){
  char buff[10];
  char syn_ack[] = "SYN_ACK";
  *to_client = open("PP",O_WRONLY,0);
  read(from_client,buff,sizeof(buff));
  if(strcmp(buff,"SYN") != 0){
    printf("Server handshake failed\n");
  }
  write(*to_client,syn_ack,sizeof(syn_ack));
  read(from_client,buff,sizeof(buff));
  if(strcmp(buff,"ACK") != 0){
    printf("Server handshake failed\n");
  }
}

/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  mkfifo("PP",0666);
  *to_server = open("WKP",O_WRONLY,0);
  int from_server = open("PP",O_RDONLY,0);
  char syn[] = "SYN";
  char ack[] = "ACK";
  char buff[10];
  write(*to_server,syn,sizeof(syn));
  read(from_server,buff,sizeof(buff));
  if(strcmp(buff,"SYN_ACK") != 0){
    printf("Client handshake failed\n");
  }
  write(*to_server,ack,sizeof(ack));
  remove("PP");
  printf("Client successfully connected\n");
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