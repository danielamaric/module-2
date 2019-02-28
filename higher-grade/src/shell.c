#include "parser.h"

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>

#define READ  0
#define WRITE 1


void fork_error() {
  perror("fork() failed)");
  exit(EXIT_FAILURE);
}

void wait_for_all_cmds(int n) {
  // Not implemented yet!

  for (int i = 0; i < n; i++) {
    wait(NULL);
  }
}

void fork_cmd(char* argv[], int *input, int *output) {
  pid_t pid;

  switch (pid = fork()) {
    case -1:
      fork_error();
    case 0:
      //printf("\n>>>> %d, COMMAND: %s, READ FROM: %d\n", getpid(), argv[0], input[0]);
      //printf("\n>>>> %d, COMMAND: %s, WRITE TO: %d\n", getpid(), argv[0], output[1]);
      if (output[0] != 0) close(output[0]);
      if (input [1] != 1) close(input[1]);
      dup2(output[1], 1);
      dup2(input[0], 0);
      
      execvp(argv[0], argv);
      perror("execvp");
      exit(EXIT_FAILURE);
    default:
      //printf("\n>>>> WE ARE CLOSING: %d\n", output[1]);
       if (output[1] != 1) close(output[1]);
      break;
  }
}

void fork_cmds(char* argvs[MAX_COMMANDS][MAX_ARGV], int n) {
  int input[2] = {0, 1};
  int output[2] = {0, 1};
  
  for (int i = 0; i < n; i++) {

    if (i+1 < n) {
      int fd[2];
      
      if (pipe(fd) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
      }
      output[0] = fd[0];
      output[1] = fd[1];
      fork_cmd(argvs[i], input, output);
      input[0] = fd[0];
      input[1] = fd[1];
      
    }
    else {
      output[0] = 0;
      output[1] = 1;
      fork_cmd(argvs[i], input, output);
    }
  }
  
  /* if (n != 1) {
    printf("\n>>>> WE ARE CLOSING: %d\n", first);
    close(first);
    }*/

  wait_for_all_cmds(n);
}

void get_line(char* buffer, size_t size) {
  getline(&buffer, &size, stdin);
  buffer[strlen(buffer)-1] = '\0';
}

int main() {
  int n; //Antalet kommandon. 
  char* argvs[MAX_COMMANDS][MAX_ARGV]; //Här lagras komandon i parsen.
  size_t size = 128; //Maxlängd på input;
  char line[size];

  while(true) {
    printf(" >> ");
    get_line(line, size);

    n = parse(line, argvs);

    // Debug printouts.
    printf("%d commands parsed.\n", n);
    print_argvs(argvs);

    fork_cmds(argvs, n);
    wait_for_all_cmds(n);
  }

  exit(EXIT_SUCCESS);
}
