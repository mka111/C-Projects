#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include "bbuff.h"
#include <pthread.h>
#include<stdbool.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void* Bufferdata[BUFFER_SIZE]; //BUFFER_SIZE DEFINED IN BUFF.H
sem_t haveData;
sem_t haveSpace;
int i;
void bbuff_init(void) //intialization constructor
{
  sem_init(&haveSpace, 0 , BUFFER_SIZE);
 sem_init(&haveData, 0 , 0);
 pthread_mutex_init(&mutex,NULL);
}

void bbuff_blocking_insert(void* item)
{
  sem_wait(&haveSpace);
 //Critical section
  pthread_mutex_lock(&mutex);
  Bufferdata[i] = item;
  i++;
  //end of critical section
  pthread_mutex_unlock(&mutex);
  sem_post(&haveData);
}
void* bbuff_blocking_extract(void)
{
 void* item;
 sem_wait(&haveData);
 //critical section
 pthread_mutex_lock(&mutex);
 i--;
 item = Bufferdata[i];
 pthread_mutex_unlock(&mutex);
 sem_post(&haveSpace);
 return item;
}

_Bool bbuff_is_empty(void)
{
  if(i > 0)
  {
    return false;
  }
  return true;
}
