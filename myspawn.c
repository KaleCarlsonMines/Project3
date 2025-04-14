#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

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

    if(WIFEXITED(status)) {
        printf("Spawned process exited with status %d\n", WEXITSTATUS(status));
    }

    // Destroy spawn attributes
    posix_spawnattr_destroy(&attr);

    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    // Set flags if needed, for example, to specify the scheduling policy
    // posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSCHEDULER);

    return spawnWait(argv[0], &argv[0]);
}
