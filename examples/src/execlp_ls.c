#include <unistd.h> // execlp()
#include <stdio.h>  // perror()
#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE

int main(void) {
  execlp("ls", "ls", NULL);
  perror("Return from execlp() not expected");
  exit(EXIT_FAILURE);
}
