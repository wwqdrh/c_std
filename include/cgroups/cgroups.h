#ifndef CGROUPS_CGROUPS_H
#define CGROUPS_CGROUPS_H

#include <fcntl.h>

#define CGROUPS_MEMORY_MAX "1G"
#define CGROUPS_CPU_WEIGHT "256"
#define CGROUPS_PIDS_MAX "64"
#define CGROUPS_CGROUP_PROCS "cgroup.procs"
enum { CGROUPS_CONTROL_FIELD_SIZE = 256 };

int cgroups_init(char* hostname, pid_t pid);
int cgroups_free(char* hostname);

#endif
