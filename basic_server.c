#include "pipe_networking.h"


int main() {
  int to_client;
  int from_client;

  from_client = server_handshake( &to_client );
  char buff[256];
  read(from_client,buff,sizeof(buff));
  printf("%s\n",buff);
}
