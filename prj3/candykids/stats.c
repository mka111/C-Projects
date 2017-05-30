#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<semaphore.h>
#include "stats.h"
#include<unistd.h>
#include<time.h>
//Structure to keep track of number of Values of Candy-factory.
typedef struct{
int factoryNumber;
double TotalDelay;
int made;
int eaten;
double minDelay;
double maxDelay;
double AvgDelay;
}stats_candy;


int TotalProducers = 0; //global variable for number of producers to help in use in Display function.
stats_candy* array; //global variable
void stats_init(int num_producers)
{
  array = malloc(sizeof(stats_candy)*num_producers);
  for(int i=0; i<num_producers; i++)
  {
    array[i].TotalDelay = 0;
    array[i].made = 0;
    array[i].eaten = 0;
    array[i].minDelay = 0;
    array[i].maxDelay = 0;
    array[i].AvgDelay = 0;
  }
  TotalProducers = num_producers; //store the value in global varibale here
}

void stats_cleanup(void)
{
  free(array);
}

void stats_record_produced(int factory_number)
{
  array[factory_number].made = array[factory_number].made + 1;
}

void stats_record_consumed(int factory_number, double delay_in_ms)
{
  array[factory_number].eaten+=1;
  if(array[factory_number].minDelay > delay_in_ms)
  {
    array[factory_number].minDelay = delay_in_ms;
  }
  if(array[factory_number].maxDelay < delay_in_ms)
  {
    array[factory_number].maxDelay = delay_in_ms;
  }
  if(array[factory_number].minDelay == 0) //To make sure MinDelay is not only with intialized value
  {
    array[factory_number].minDelay = array[factory_number].maxDelay = array[factory_number].TotalDelay
      = array[factory_number].AvgDelay = delay_in_ms;
  }
  array[factory_number].TotalDelay+=delay_in_ms;
  array[factory_number].AvgDelay = array[factory_number].TotalDelay / array[factory_number].eaten;

}
void stats_display(void)
{
  printf("\nStatistics:\n");
  printf("%8s%10s%10s%17s%17s%17s\n", "Factory#","#Made","#Eaten","Min Delay[ms]","Avg Delay[ms]",    "Max Delay[ms]");
  for(int i=0; i<TotalProducers; i++)
  {
    printf("%8d%8d%8d%15.5f%18.5f%18.5f\n", i , array[i].made , array[i].eaten , array[i].minDelay , array[i].AvgDelay , array[i].maxDelay);
  }
  for(int j=0; j<TotalProducers; j++)
  {
    if(array[j].eaten != array[j].made)
    {
      printf("\nERROR: Mismatch between number made and eaten\n");
      break;
    }
  }
}
