#include <stdio.h>    // puts(), printf(), perror(), getchar()
#include <stdlib.h>   // exit(), EXIT_SUCCESS, EXIT_FAILURE
#include <unistd.h>   // getpid(), getppid(),fork()
#include <sys/wait.h> // wait()

#define READ  0
#define WRITE 1

void child_a(int fd[]) {

  close(fd[0]);
  dup2(fd[1], 1);
  execlp("ls", "ls", "-F", "-1", NULL);



}

void child_b(int fd[]) {

  close(fd[1]);
  dup2(fd[0], 0);
  execlp("nl", "nl", NULL);

 

}

int main(void) {
  int fd[2];
  if (pipe(fd) == -1){
    perror("Could not create pipe!");
    exit(EXIT_FAILURE);
  }

  pid_t child_a_pid = fork();
  
  if (child_a_pid == 0){
    child_a(fd);
    
    
  } else {
    pid_t child_b_pid = fork();
    if (child_b_pid == 0){
      child_b(fd);
      
    } else {
      //parent
      //close(fd[0]);
      close(fd[1]);
      wait(NULL); // could use &child_a_pid instead of null 
      wait(NULL);
      
    }
  }

}
