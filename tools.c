#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "logger.h"

pid_t exec_cmd(const char *cmd)
{
    logger(DEBUG, "exec_cmd: %s", cmd);

    pid_t pid;
    pid = fork();

    if (pid == 0) {
        execlp("/bin/sh", "sh", "-c", cmd, NULL);
        logger(CRITICAL, "exec_cmd: execld failed");
        exit(EXIT_FAILURE); // we should never reach this line
    }

    return pid;
}
