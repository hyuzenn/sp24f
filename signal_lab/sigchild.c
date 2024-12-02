#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include<stdlib.h>

#define NUM 8


int ccount = 0;

void child_handlerl(int sig){
  int status;

  pid_t pid = wait(&status);
  ccount--;

  fprintf(stdout, "Received signal %d from process %d\n", sig, pid);
}

int main(int argc, char* argv[]){ 
  pid_t pid[NUM];
  int i, status;

  ccount = NUM;

  signal(SIGCHLD, child_handlerl);

  for (i = 0; i < NUM; i++){
     if((pid[i] = fork()) == 0){
      sleep(2);
      exit(0);
    }
  }
  while(ccount > 0)
    pause();
}