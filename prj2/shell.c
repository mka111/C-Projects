#define _POSIX_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <ctype.h>
#define COMMAND_LENGTH 1024
#define NUM_TOKENS (COMMAND_LENGTH / 2 + 1)
#define HISTORY_DEPTH 1024

char history[HISTORY_DEPTH][COMMAND_LENGTH];
int standardlength=0;
int totalHistory=0;

int tokenize_command(char *buff, char *token[])
{
  char *temp = strtok(buff, " ");
  int i=0;
  token[i] = temp;
  while(temp!=NULL)
  {
    i++;
    temp = strtok(NULL, " ");
    token[i] = temp;
  }
  token[i] = NULL;
  standardlength= i;
return i;
}


void read_command(char *buff, char *tokens[], _Bool *in_background)
{
	*in_background = false;

	// Read input
  int length = read(STDIN_FILENO, buff, COMMAND_LENGTH-1);
	if ( (length < 0) && (errno !=EINTR) )
	{
		perror("Unable to read command. Terminating.\n");
		exit(-1); /* terminate with error */
	}
	// Null terminate and strip \n.
	buff[length] = '\0';
	if (buff[strlen(buff) - 1] == '\n') {
		buff[strlen(buff) - 1] = '\0';
	}
	// Tokenize (saving original command string)
	int token_count = tokenize_command(buff, tokens);
       // printf("This is token count %d \n", token_count);
	if (token_count == 0) {
		return;
	}
	// Extract if running in background:
	if (token_count > 0 && strcmp(tokens[token_count - 1], "&") == 0) {
		*in_background = true;
		tokens[token_count - 1] = 0;
	}
}

int main(int argc, char* argv[])
{
  char input_buffer[COMMAND_LENGTH];
  char *tokens[NUM_TOKENS];
  printf("this here\n");
  int k = 0;
  int f =totalHistory;
  while(true) {
    write(STDOUT_FILENO, getcwd(input_buffer, 1024), strlen(getcwd(input_buffer, 1024)));
    write(STDOUT_FILENO, ">" , strlen(">"));
    _Bool in_background = false;
    read_command(input_buffer, tokens, &in_background);
    int sum =0;
    for(int z = 0; z< standardlength; z++)
    {
      sum =sum + strlen(tokens[z]);
    }
    sum =sum + (standardlength-1);
    int flag=0;
    if(tokens[0][0]=='!')
    {
      if(tokens[0][1] == '!' || isdigit(tokens[0][1]))
      {
        flag=1;
      }
    }
    if(flag == 0)
    {
      for(int j=0; j<sum; j++)
      {
        if(tokens[0][j]=='\0')
        history[f][j] = ' ';
        else
        history[f][j] = tokens[0][j];
      }
      f++;
    }
    if(tokens[0]!=NULL)
    {
      if(strcmp(tokens[0],"exit")==0)
      {
        kill(0,SIGINT);
      }
      else if(strcmp(tokens[0], "!!")==0)
      {
        if(f>0)
        {
          write(STDOUT_FILENO, history[f-1], strlen(history[f-1]));
          write(STDOUT_FILENO, "\n", strlen("\n"));
        }
        else
        {
          write(STDOUT_FILENO, "Error: No commands to show\n",strlen("Error: No commands to show\n"));
        }
      }
      else if(tokens[0][0] == '!')
      {
        int numberLine = atoi(&tokens[0][1]);
        if(numberLine > f-1 || numberLine <= 0)
        {
          write(STDOUT_FILENO,"Invalid exceution of command number\n", strlen("Invalid exceution of command number\n"));
        }
        else
        {
          write(STDOUT_FILENO, "\n", strlen("\n"));
          write(STDOUT_FILENO, history[numberLine-1], strlen(history[numberLine-1]));
          write(STDOUT_FILENO, "\n", strlen("\n"));
        }
      }
      else if(strcmp(tokens[0], "pwd")==0)
      {
        printf("Current directory %s \n", getcwd(input_buffer, 1024));
      }
      else if(strcmp(tokens[0], "history")==0)
      {
        int length = strlen(history[0]);
        int index;
        if(f>9)
        {
          index = f-10;
        }
        else
        {
          index = 0;
        }
        while(index<f){
          int now = index+1;
          char str[now];
          sprintf(str, "%d\t", now);
          write(STDOUT_FILENO, str, strlen(str));
          write(STDOUT_FILENO, history[index], strlen(history[index]));
          printf("\n");
          index++;
        }
      }

      else if(strcmp(tokens[0], "cd")==0)
     {
       int returnValue;
       returnValue = chdir(tokens[1]);
       if(returnValue == -1)
       {
         write(STDOUT_FILENO, "Directory cannot be changed\n" , strlen("Directory cannot be changed\n "));
       }
     }
   }
     pid_t i = fork();
     if( i==0)
     {
       execvp(tokens[0], tokens);
     }
     if (!in_background) {
     waitpid(i, NULL, 0);
   }
 }
 return 0;
}
