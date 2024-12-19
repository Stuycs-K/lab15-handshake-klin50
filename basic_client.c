#include "pipe_networking.h"

int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  int buff[256];
  fgets(buff,255,stdin);
  write(to_server,buff,sizeof(buff));
  
}
