#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include<stdbool.h>
#include "bbuff.h"
#include "stats.h"
_Bool stop_thread = false; //Global variable to help ending the thread

typedef struct {
int factory_number; 
double time_stamp_in_ms;
}candy_t;

double current_time_in_ms(void)
{
  struct timespec now;
  clock_gettime(CLOCK_REALTIME, &now);
  return now.tv_sec *1000.0 + now.tv_nsec/1000000.0;
}
//Candy Factory Thread
void* candyFactoryThread(void *t)
{
  int Number = *((int *) t);
  while(!stop_thread)
  {
    int NumberOfsec = rand()%4;
    printf("\tFactory %d ships candy & waits %ds \n", Number, NumberOfsec);
    candy_t* candyItem = malloc(sizeof(candy_t));
    candyItem->factory_number = Number;
    candyItem->time_stamp_in_ms = current_time_in_ms();
    stats_record_produced(Number); //to give information to get correct statistics.
    bbuff_blocking_insert(candyItem);
    sleep(NumberOfsec);
  }
  printf("Candy-factory %d done \n", Number);
  pthread_exit(NULL);
}
//Kid Thread
void* kidThread(void *t)
{
  int Number = *((int *) t);
  while(true)
  {
    candy_t* candyItem = (candy_t*) bbuff_blocking_extract();
    int factory_number = candyItem->factory_number;
    double delay_in_ms = (current_time_in_ms()) - (candyItem->time_stamp_in_ms);
    stats_record_consumed(factory_number, delay_in_ms);
    int NumberOfsec = rand()%2;
    printf("Kids number %d had the candies and waits for %ds\n", Number, NumberOfsec);
    sleep(NumberOfsec);
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
 int kids =0;
 int factory;
 int seconds = 0;
 if(argc == 0 || argc!= 4)
 {
  printf("Need 3 arguments!!!\n");
  return 1;
 }
//Extract Arguments
 if(argc == 4){
   factory = atoi(argv[1]);
   kids = atoi(argv[2]);
  seconds = atoi(argv[3]);
}
//Intialize modules
bbuff_init();
stats_init(factory);

//Launch factory threads
pthread_t factoryIds[factory];
int factoryMap[factory];
for(int i=0; i<factory; i++)
{
  factoryMap[i] = i;
  pthread_create(&factoryIds[i], NULL, candyFactoryThread , &factoryMap[i]);
}

//Launch kid thread
pthread_t kidIds[kids];
for(int j=0; j<kids; j++)
{
  pthread_create(&kidIds[j], NULL, kidThread, &j);
}

//Wait for requested time
for(int t=0; t<seconds; t++)
{
 sleep(1);
 printf("Time %ds\n", t);
}

//Stop factory threads
stop_thread = true;
for(int i=0; i<factory; i++)
{
  pthread_join(factoryIds[i], NULL);
}

//Wait until no candy
while(bbuff_is_empty() == false){
  printf("Waiting for all candy to be consumed\n");
  sleep(1);
}

//Stop kid thread
for(int j=0; j<kids ; j++)
{
 pthread_cancel(kidIds[j]);
 pthread_join(kidIds[j],NULL);
}

//Print Statistics
stats_display();
//CleanUp any allocated memory.
stats_cleanup();
return 0;
}
