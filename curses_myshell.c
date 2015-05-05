#include<stdio.h>
#include<stdlib.h>
#include<wait.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>
#include<ncurses.h>

void parse(char *cmd, char **args)
{
     while (*cmd != '\0') {       
          while (*cmd == ' ' || *cmd == '\t' || *cmd == '\n')
               *cmd++ = '\0';     
          *args++ = cmd;          
          while (*cmd != '\0' && *cmd != ' ' && 
                 *cmd != '\t' && *cmd != '\n') 
               cmd++;             
     }
     *args = '\0';                
}

void _execute(char **args)
{
	pid_t  pid;
    int    status;
     
    if ((pid = fork()) < 0) {     
         printw("*** ERROR: forking child process failed");
          exit(1);
     }
    else if (pid == 0) {          
          if (execvp(*args, args) < 0) {     
               printw("*** ERROR: exec failed");
               exit(1);
          }
     }
    else {                                  
          while (wait(&status) != pid)      
               ;
    }
}


int main()
{
	char cmd[100];
	char hist[10][100];
	char *args[10];
	
	initscr();
	raw();
	keypad(stdscr,TRUE);
	printw("****Tanay's SHELL******");
	int i=0;
	while(1)
	{
		printw("tsh>");
		getstr(cmd);
		
		strcpy(hist[++i % 10],cmd);
		
		if(!strcmp(cmd,"hist"))
		{
			i--;
			if(i<0)i=0;
			strcpy(cmd,hist[i-- % 10]);
			printf("%s",cmd);
				
		}
		if(!strcmp(cmd,"exit"))
		{
			exit(1);
		}
		parse(cmd,args);
		_execute(args);
	}
	
	return 0;
	
}
