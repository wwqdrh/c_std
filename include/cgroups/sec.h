#ifndef CGROUPS_SEC
#define CGROUPS_SEC

#include <errno.h>

#define SEC_SCMP_FAIL SCMP_ACT_ERRNO(EPERM)
int sec_set_caps(void);
int sec_set_seccomp(void);

#endif
