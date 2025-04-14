#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
	// TODO
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise

    for(int i = 0; i < (sizeof(allowed) / sizeof(allowed[0])); i++){
        if(strcmp(cmd, allowed[i]) == 0) {
            return 1;
        }
    }    
	return 0;
}

int main() {

    // TODO
    // Add variables as needed

    char myspawn_path[4096];

    realpath("./myspawn", myspawn_path);

    char line[256];

    while (1) {

	fprintf(stderr,"rsh>");

	if (fgets(line,256,stdin)==NULL) continue;

	if (strcmp(line,"\n")==0) continue;

	line[strlen(line)-1]='\0';

	// TODO
	// Add code to spawn processes for the first 9 commands
	// And add code to execute cd, exit, help commands
	// Use the example provided in myspawn.c

    //tokenize arguments
    char *tokens[10];
    int i = 0;
    //tokens[0] = "./myspawn";
    char *token = strtok(line, " ");
    while (token && i<9) {
        tokens[i] = token;
        token = strtok(NULL, " ");
	i++;
    }
    tokens[i] = NULL;

    //input handler
    //checks for shell commands first
    //
    if(strcmp(tokens[0], "exit") == 0){
        return 0;
    } else if(strcmp(tokens[0], "help") == 0){
	    printf("The allowed commands are:\n");
        for(int j = 0; j < (sizeof(allowed) / sizeof(allowed[0])); j++){
            printf("%d: %s\n", j + 1, allowed[j]);
        }
	continue;
    }else if(strcmp(tokens[0], "cd") == 0){
        if(tokens[1]){
            if(chdir(tokens[1]) != 0) perror("cd failed");
        } else if(tokens[2]){
		printf("-rsh: cd: too many arguments");
    	}else {
            printf("cd missing arguments\n");
        }
	continue;
    } else if(!isAllowed(tokens[0])) {
        printf("NOT ALLOWED!\n");
        continue;
    } else{

	pid_t pid = fork();
    	if(pid == 0){
            execvp(myspawn_path, tokens);
            perror("spawn failed");
            exit(EXIT_FAILURE);
    	} else if(pid > 0){
            waitpid(pid, NULL, 0);
    	} else {
            perror("spawn failed");
    	}
    
	}
   }

    return 0;
}
