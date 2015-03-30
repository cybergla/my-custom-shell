#include<stdio.h>
#include<stdlib.h>
#include<wait.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>

void parse(char *cmd, char **args)
{
     while (*cmd != '\0') {       /* if not the end of line ....... */ 
          while (*cmd == ' ' || *cmd == '\t' || *cmd == '\n')
               *cmd++ = '\0';     /* replace white spaces with 0    */
          *args++ = cmd;          /* save the argument position     */
          while (*cmd != '\0' && *cmd != ' ' && 
                 *cmd != '\t' && *cmd != '\n') 
               cmd++;             /* skip the argument until ...    */
     }
     *args = '\0';                 /* mark the end of argument list  */
}

void _execute(char **args)
{
	pid_t  pid;
    int    status;
     
    if ((pid = fork()) < 0) {     /* fork a child process           */
         printf("*** ERROR: forking child process failed\n");
          exit(1);
     }
    else if (pid == 0) {          /* for the child process:         */
          if (execvp(*args, args) < 0) {     /* execute the command  */
               printf("*** ERROR: exec failed\n");
               exit(1);
          }
     }
    else {                                  /* for the parent:      */
          while (wait(&status) != pid)       /* wait for completion  */
               ;
    }
}

int main()
{
	char cmd[100];
	char *args[10];
	printf("\n****Tanay's SHELL******");
	while(1)
	{
		printf("\ntsh>");
		gets(cmd);
		if(!strcmp(cmd,"exit"))
		{
			exit(1);
		}
		parse(cmd,args);
		_execute(args);
	}
	
	return 0;
	
}
