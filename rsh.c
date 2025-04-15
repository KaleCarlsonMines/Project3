
#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int spawnWait(const char *command, char *const argv[]){
    pid_t pid;
    int status;
    posix_spawnattr_t attr;

    //initialize spawn attributes
    posix_spawnattr_init(&attr);

    //spawn a new process
    if (posix_spawnp(&pid, command, NULL, &attr, argv, environ) != 0) {
        perror("spawn failed");
        exit(EXIT_FAILURE);
    }

    //wait for the spawned process to end
    if(waitpid(pid, &status, 0) == -1){
        perror("waitpid failed");
        exit(EXIT_FAILURE);
    }
/*
    if(WIFEXITED(status)) {
        printf("Spawned process exited with status %d\n", WEXITSTATUS(status));
    }
*/
    // Destroy spawn attributes
    posix_spawnattr_destroy(&attr);
}

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

    	spawnWait(tokens[0], &tokens[0]);
    
	}
   }
   
    return 0;
}

