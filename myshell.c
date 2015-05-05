#include<stdio.h>
#include<stdlib.h>
#include<wait.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>

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
         printf("ERROR: forking child process failed\n");
          exit(1);
     }
    else if (pid == 0) {          
          if (execvp(*args, args) < 0) {     
               printf("ERROR: could not find command\n");
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
	char wd_path[1024];
	if(!chdir("/home/tanay"));
	else
		printf("ERROR: could not change working directory\n");
	
	printf("\n**********Tanay's SHELL*************");
	int i=0;
	while(1)
	{
		printf("\ntsh>");
		gets(cmd);
		
		strcpy(hist[++i % 10],cmd);
		
		if(!strcmp(cmd,":hist"))
		{
			i--;
			if(i<0)i=0;
			strcpy(cmd,hist[i-- % 10]);
			printf("%s",cmd);
			while(getchar() != '\n')
				;//DO NOTHING	
		}
		if(!strcmp(cmd,"exit"))
		{
			exit(1);
		}
		if(!strcmp(cmd,"wd"))
		{
			if(getcwd(wd_path,sizeof(wd_path))!= NULL)
				printf("%s",wd_path);
			else
				printf("ERROR: could not get current working directory\n");
			continue;
		}
		parse(cmd,args);
		if(!strcmp(args[0],"cd"))
		{
			if(!chdir(args[1]));
			else
			printf("ERROR: could not change working directory\n");
			continue;
		}
		_execute(args);
	}
	
	return 0;
	
}
