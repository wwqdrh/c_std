#define _GNU_SOURCE 1
#include <unistd.h>
#include <signal.h>
#include <sched.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "cgroups/container.h"
#include "cgroups/mount.h"
#include "cgroups/sec.h"
#include "cgroups/user.h"
#include "lib/log.h"

int container_start(void* arg) {
    container_config* config = arg;
    log_debug("starting container");
    log_debug("setting hostname, mounts, user namespace, capabilities and syscalls...");

    if (sethostname(config->hostname, strlen(config->hostname)) ||
        mount_set(config->mnt) || user_namespace_init(config->uid, config->fd) ||
        sec_set_caps() || sec_set_seccomp()) {
        log_debug("failed to set properties");
        close(config->fd);
        return -1;
    }
    return 0;

    log_debug("closing container socket...");
    if (close(config->fd)) {
        log_error("failed to close container socket: %m");
        return -1;
    }

    log_debug("executing command '%s %s' from directory '%s' in container...",
                        config->cmd, config->arg, config->mnt);
    log_info("### BARCONTAINER STARTING - type 'exit' to quit ###");
    char* argv[] = {config->arg, NULL};
    if (execve(config->cmd, argv, NULL)) {
        log_error("failed to execve '%s %s': %m", config->cmd, argv);
        return -1;
    }
    log_debug("container started...");
    return 0;
}

int container_init(container_config* config, char* stack) {
    int container_pid = 0;
    int flags = CLONE_NEWNS | CLONE_NEWCGROUP | CLONE_NEWPID | CLONE_NEWIPC |
                      CLONE_NEWNET | CLONE_NEWUTS;
    log_debug("cloning process...");
    if ((container_pid = clone(container_start, stack, flags | SIGCHLD, config)) == -1) {
        log_error("failed to clone: %m");
        return 1;
    }
    log_debug("resetting container socket...");
    close(config->fd);
    config->fd = 0;
    return container_pid;
}

int container_wait(int container_pid) {
    int container_status = 0;
    log_debug("waiting for container_pid %d...", container_pid);
    waitpid(container_pid, &container_status, 0);
    log_debug("container_pid %d exited", container_pid);
    return WEXITSTATUS(container_status);
}

void container_stop(int container_pid) {
    log_debug("calling kill for container_pid %d...", container_pid);
    if (kill(container_pid, SIGKILL)) {
        log_error("failed to kill container_pid %d: %m", container_pid);
    }
    log_debug("container_pid %d killed", container_pid);
}
