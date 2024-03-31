#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "cgroups/cgroups.h"
#include "lib/log.h"

struct cgroups_setting {
    char name[CGROUPS_CONTROL_FIELD_SIZE];
    char value[CGROUPS_CONTROL_FIELD_SIZE];
};

int cgroups_init(char* hostname, pid_t pid) {
    char cgroup_dir[PATH_MAX] = {0};
    struct cgroups_setting* procs_setting = &(struct cgroups_setting){
        .name = CGROUPS_CGROUP_PROCS,
        .value = ""
    };
    if (snprintf(procs_setting->value, CGROUPS_CONTROL_FIELD_SIZE, "%d", pid) == -1) {
        log_error("failed to setup cgroup.procs setting: %m");
        return -1;
    }

    struct cgroups_setting* cgroups_setting_list[] = {
        &(struct cgroups_setting){
            .name = "memory.max",
            .value = CGROUPS_MEMORY_MAX
        },
        &(struct cgroups_setting){
            .name = "cpu.weight",
            .value = CGROUPS_CPU_WEIGHT
        },
        &(struct cgroups_setting){
            .name = "pids.max",
            .value = CGROUPS_PIDS_MAX
        },
        procs_setting,
        NULL
    };
    log_debug("setting cgroups...");
    if (snprintf(cgroup_dir, sizeof(cgroup_dir), "/sys/fs/cgroup/%s", hostname) ==
                  -1) {
        log_error("failed to setup path: %m");
        return -1;
    }

    log_debug("creating %s...", cgroup_dir);
    if (mkdir(cgroup_dir, S_IRUSR | S_IWUSR | S_IXUSR)) {
        log_error("failed to mkdir %s: %m", cgroup_dir);
        return -1;
    }

    for(struct cgroups_setting** setting = cgroups_setting_list; *setting; setting++) {
        char setting_dir[PATH_MAX] = {0};
        int fd = 0;

        log_info("setting %s to %s...", (*setting)->name, (*setting)->value);
        if (snprintf(setting_dir, sizeof(setting_dir), "%s/%s", cgroup_dir,
            (*setting)->name) == -1) {
            log_error("failed to setup path: %m");
            return -1;
        }

        log_debug("opening %s...", setting_dir);
        if ((fd = open(setting_dir, O_WRONLY)) == -1) {
            log_error("failed to write %s: %m", setting_dir);
            close(fd);
            return -1;
        }

        log_debug("writing %s to setting", (*setting)->value);
        if (write(fd, (*setting)->value, strlen((*setting)->value)) == -1) {
            log_error("failed to write %s: %m", setting_dir);
            close(fd);
            return -1;
        }

        log_debug("closing %s...", setting_dir);
        if (close(fd)) {
            log_error("failed to close %s: %m", setting_dir);
            return -1;
        }
    }
    log_debug("cgroups set");
    return 0;
}

int cgroups_free(char* hostname) {
    char dir[PATH_MAX] = {0};
    log_debug("freeing cgroups...");
    if (snprintf(dir, sizeof(dir), "/sys/fs/cgroup/%s", hostname) == -1) {
        log_error("failed to setup paths: %m");
        return -1;
    }
    log_debug("removing %s...", dir);
    if (rmdir(dir)) {
        log_error("failed to rmdir %s: %m", dir);
        return -1;
    }

    log_debug("cgroups released");
    return 0;
}
