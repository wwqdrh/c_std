#ifndef CGROUPS_CONTAINER_H
#define CGROUPS_CONTAINER_H

#include <sys/types.h>

enum {
	CONTAINER_STACK_SIZE = (1024 * 1024),
};

typedef struct {
	uid_t uid;
	int fd;
	char* hostname;
	char* cmd;
	char* arg;
	char* mnt;
} container_config;

int container_init(container_config* config, char *stack);
int container_wait(int container_pid);
void container_stop(int container_pid);

#endif
