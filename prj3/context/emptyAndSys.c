//The following program measure the cost of empty Function and system call.
#define _POSIX_C_SOURCE 199309L
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<stdbool.h>
#include<semaphore.h>
#include<time.h>
#define BILLION 1000000000
#define READ  0
#define WRITE 1

void EmptyFunction()
{
}
//------empty function call cost
void main(void) {

  struct timespec emptystart;
  struct timespec emptystop;
  double sum =0;
  double avg;
  for(int i=0; i<10000; i++) //repeating the function 1000 times to take average in the end.
  {
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &emptystart);
    EmptyFunction();
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &emptystop);
    double time = (emptystop.tv_sec - emptystart.tv_sec)*(BILLION) + (double)(emptystop.tv_nsec -
                       emptystart.tv_nsec);
    sum = sum+time;
  }
  avg = sum/10000;
  printf("The cost of calling empty function is %lf nanoseconds\n" , avg);

//-----system call cost
struct timespec systemstart;
struct timespec systemstop;
double avg1;
double sum1 = 0;
for(int i=0; i<10000; i++)
{
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &systemstart);
  getpid();
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &systemstop);
  double time1 = (systemstop.tv_sec - systemstart.tv_sec)*(BILLION) + (double)(systemstop.tv_nsec -
                     systemstart.tv_nsec);
  sum1 = sum1+time1;
}
avg1 = sum1/10000;
printf("The cost of calling system function is %lf nanoseconds\n" , avg1);
}
