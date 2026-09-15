#define _GNU_SOURCE
#include <sys/mount.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static void mount_fs(
    const char *source,
    const char *target,
    const char *fstype)
{
    if (mount(source, target, fstype, 0, NULL) == -1) {
        perror(target);
        exit(EXIT_FAILURE);
    }
}

int main(void)
{
    mount_fs("devtmpfs","/dev","devtmpfs");
    mount_fs("proc","/proc","proc");
    mount_fs("sysfs","/sys","sysfs");

    setenv("PATH","/bin",1);

    pid_t pid = fork();

    if (pid == 0) {
        execl("/bin/ksh","ksh",(char *)NULL);

        perror("exec ksh");
        _exit(127);
    }

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    for (;;) {
        wait(NULL);
    }

    return 0;
}
