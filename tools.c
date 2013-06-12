#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

pid_t exec_cmd(const char *cmd)
{
    pid_t pid;
    pid = fork();

    if (pid == 0) {
        execlp("/bin/sh", "sh", "-c", cmd, NULL);
        exit(EXIT_FAILURE); // we should never reach this line
    }

    return pid;
}
