#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#define SIZEBR 2000

int main()
{
	// Reading the name of file of input.
	char name[20];
	printf("Enter name of file:");
	scanf("%s", name);

	// Creating pipe
	int fd[2];
	if (pipe(fd) == -1){
		perror("Error of creating pipe");
		return -1;
	}

	// Creating file
	int file = open(name, O_RDONLY  | O_CREAT, 0777);
	if(file == -1){
		perror("Error of opening file!");
		return -1;
	}

	// Creating child process
	int id = fork();
	if (id == -1)
	{
		perror("fork error");
		return -1;
	}
	if (id == 0)
	{ // Child process
		int file2 = dup2(file, STDIN_FILENO);
		if(dup2(fd[1],STDOUT_FILENO) == -1){
			perror("dup2 err");
			return -1;
		}
		close(file);
		close(fd[0]);
		int err = execlp("./child", "child", NULL);
		if(err == -1){
			perror("Eror of executing child programm");
			return -1;
		}
	}
	else
	{ //Parent process
		close(fd[1]);
		float res;
		if(dup2(fd[0], STDIN_FILENO) == -1){
			perror("dup2 err");
			return -1;
		};
		while(scanf("%f", &res) >  0){
			printf("%f ", res);
		}
		close(file);
		close(fd[0]);
		wait(NULL);
	}
	return 0;
}
