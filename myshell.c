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

int check_pipe(char **args)
{
	int i=0;
	while(args[i] != '\0')
	{
		if(*args[i] == '|')
		return 1;
		
		i++;
	}
	return 0;
}

void exec_pipe(char **args)
{
	pid_t pid;
	int pfds[2];
	char *arg1[10],*arg2[10];
	int i=0;
	//printf("pipe here");
	while(*args[i] != '|')
	{
		arg1[i] = args[i];
		i++;
	}
	//printf("\n%s",args[i]);
	arg1[i] = '\0';
	i++;
	int j =0;
	while(args[i] != NULL)
	{
		arg2[j] = args[i];
		//printf("\n%s",arg2[j]);
		j++;
		i++;
	}
	arg2[j] = '\0';
	pipe(pfds);
	pid =  fork();
	if (pid == 0) {     // child process - first cmd - redirect stdout		
	    close(1);       // close normal stdout 
	    dup(pfds[1]);   // make stdout same as pfds[1] 
	    close(pfds[0]); 
	    if (execvp(*arg1, arg1) < 0) {     
	       printf("ERROR: could not find command\n");
	       exit(1);
	  }
	    
	} else { 	    // parent process - second cmd - redirect stdin	
	    wait(&pid);
	    close(0);       // close normal stdin 
	    dup(pfds[0]);   // make stdin same as pfds[0] 
	    close(pfds[1]); 
	    if (execvp(*arg2, arg2) < 0) {     
	       printf("ERROR: could not find command\n");
	       exit(1);
	       }
	}
	
	
}

int main()
{
	char cmd[100];
	char hist[10][100];
	char *args[15];
	char wd_path[1024] = {"/home/"};
	char uid[100];
	cuserid(uid);
	strcat(wd_path,uid);
	if(chdir(wd_path))		
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
			printf("\ntsh>%s",cmd);
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
			
		
		if(check_pipe(args))
		{
			pid_t pid = fork();
			if(pid < 0)
			{
				printf("\n fork failed");
			}
			if(pid == 0)
			{
				exec_pipe(args);
			}
			else
			{
				wait(&pid);
			}
			continue;
		}
		
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
