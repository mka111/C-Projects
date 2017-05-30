//Following program is to measure the cost of thread-switching.
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
struct timespec start;
struct timespec stop;

int num = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //Lock for race conditions
void* FirstThread(void *n)
{
  while(num == 1)
  {
    pthread_mutex_lock(&mutex);
    num=0;
    pthread_mutex_unlock(&mutex);
  }
}

void* SecondThread(void *n)
{
  while(num == 0)
  {
    pthread_mutex_lock(&mutex);
    num=1;
    pthread_mutex_unlock(&mutex);
  }
}


int main()
{
  pthread_t id1, id2;
 //sPAwn threads
 clock_gettime(CLOCK_MONOTONIC, &start);
 pthread_create(&id1, NULL, FirstThread, NULL);
 pthread_create(&id2, NULL, SecondThread, NULL);
 //Join threads
 pthread_join(id1, NULL);
 pthread_join(id2, NULL);
 clock_gettime(CLOCK_MONOTONIC, &stop);
 double result = (stop.tv_sec - start.tv_sec)*(BILLION) + (double)(stop.tv_nsec -
                     start.tv_nsec);
 printf("The cost of thread-switching function is %lf nanoseconds\n" , result);
 return 0;
}
