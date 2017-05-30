//The following program is to measure the cost of process-switch
#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<stdbool.h>
#include<semaphore.h>
#include<time.h>
#include <unistd.h>
#include <string.h>
#include<errno.h>
#include<sys/wait.h>
double result = 0;
struct timespec start;
struct timespec stop;

int process()
{
//for(int j =0 ; j<10; j++){
 // for(int j=0; i<1000; j++){
 pid_t i;
    //two pipes
 int mainpipe[2];
 int childpipe[2];
 
 if (pipe(mainpipe) < 0 || pipe(childpipe) < 0) {
   perror("error");
    exit(1);
  }
  
  i = fork();

  if (i == -1) {
    printf("Fail to fork\n");
    exit(1);
  }
  if (i > 0)
  {  //main process
    char writeChar='a';
    char readChar = 'j';
    clock_gettime(CLOCK_MONOTONIC, &start);
    close(mainpipe[0]);      //close read descriptor
    write(mainpipe[1],&writeChar,sizeof(writeChar));    //write to pipe one
    close(mainpipe[1]);
    close(childpipe[1]);        //close write descriptor
    read(childpipe[0],&readChar,sizeof(readChar));
    
    close(childpipe[0]);
  }
  else if (i == 0) {
    //printf("I'm here 2");
    char writeChar='c';
    char readChar='j';
    close(mainpipe[1]);   //close write descriptor
    read(mainpipe[0],&readChar,sizeof(readChar));
    close(mainpipe[0]);
    close(childpipe[0]);       //close read descriptor
    write(childpipe[1],&writeChar,sizeof(writeChar));
    close(childpipe[1]);

    //exit(0);
    //exit(0);
   // sleep(1);
   //exit(0);
    waitpid(i, NULL, 0);
  }
  clock_gettime(CLOCK_MONOTONIC, &stop);
  result = result+(stop.tv_sec - start.tv_sec)*(1000000000) + (double)(stop.tv_nsec -
                  start.tv_nsec);
  //}
  return 0;
}

void main(void){
  process();
  printf("The cost of calling system function is %lf nanoseconds\n" , result);
}
