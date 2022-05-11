#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	int count;
	char num;
	while (read(0, &num, sizeof(char)) > 0) {
		if (num == ' ') break;
		count = count * 10 + (num - '0');
	}
	char name[count + 1];
	read (0, &name, count * sizeof(char));
	name[count] = '\0';

	int fd[2];
	if (pipe(fd) < 0) {
	return -1; 
	}

	int file = open(name, O_RDONLY);
	if (file < 0){
		return -3;
	}
	int processID = fork();
	if (processID == 0){
		dup2(file, 0);
		dup2(fd[1], 1);
		execl("child", "", NULL);
	} else {
		int status;
		wait(&status);
		if (WEXITSTATUS(status)){
			return 2;
		}
		float cur;
		read(fd[0], &cur, sizeof(float)) > 0;
		printf("%f\n", cur);
	}
	return 0; 
}