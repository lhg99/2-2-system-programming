/* mysh.c make myshell program,by Hyungi Lee,lee991229@naver.com,21-11-04 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

void cmd_help() {
	
	printf("Some examples of the built-in commands \n");
	printf("cd     : change directory \n");
	printf("exit   : exit this shell \n");
	printf("quit   : quit this shell \n");
	printf("help   : show this help \n");
	printf("?      : show this help \n");
	
}

int tokenize(char* buf, char* delims, char* tokens[], int maxtokens) {
	char* token;
	int token_count=0;
	token=strtok(buf, delims);

	while(token!=NULL && token_count < maxtokens) {
		tokens[token_count++]=token;
		token=strtok(NULL, delims);
	}
	tokens[token_count]=NULL;
	return token_count;
}

bool run(char* line) {
	char* delims=" \n\t";
	char* tokens[1024];
	int token_count, i, ba, re, fd1;
	pid_t child=0;
	token_count=tokenize(line, delims, tokens, sizeof(tokens)/sizeof(char*));

	if(strcmp(tokens[0], "exit")==0)
		return 0;
	if(strcmp(tokens[0], "help")==0 || strcmp(tokens[0], "?")==0)
		cmd_help();

	if(strcmp(tokens[0], "cd")==0) {
		if(chdir(tokens[1])!=0)
			perror("cd");
		return;
	}
	
	for(i=0;i<token_count;i++) {
		if(strcmp(tokens[i],"&")==0)
			ba++;
		if(strcmp(tokens[i],">")==0)
			re++;
	}

	if(ba) {
		if((child=fork())==0) {
			execvp(tokens[0], tokens);
			exit(0);
		}
	}
	else if(re) {
		if((child=fork())==0) {
			fd1=open(tokens[3], O_RDWR | O_CREAT, 0641);
			dup2(fd1, STDOUT_FILENO);
			execvp(tokens[0], tokens);
			exit(0);
		}
		wait(NULL);
}
	else {
		if((child=fork())==0) {
			execvp(tokens[0], tokens);
			exit(0);
		}
		wait(NULL);
	}

	memset(tokens, '\0', 1024);
	return 1;
}

int main() {
	char line[1024];
	while(1) {
		printf("%s$ ", get_current_dir_name());
		fgets(line, sizeof(line)-1, stdin);
		if(run(line)==false)
			break;
	}
	return 0;
}
