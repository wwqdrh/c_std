#ifndef CGROUPS_USER
#define CGROUPS_USER

#include <sys/types.h>

enum {
	USER_NAMESPACE_UID_PARENT_RANGE_START = 0,
	USER_NAMESPACE_UID_CHILD_RANGE_START = 10000,
	USER_NAMESPACE_UID_CHILD_RANGE_SIZE = 2000,
};

int user_namespace_init(uid_t uid, int fd);
int user_namespace_prepare_mappings(pid_t pid, int fd);

#endif
