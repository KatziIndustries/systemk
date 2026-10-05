// SPDX-License-Identifier: GPL-3.0
/* main.c
 *
 * the main function of systemk
 * that manages all subsystems
 *
 * Author:
 * JRBlockkop <jrblockkop@gmail.com>
*/

#define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/mount.h>
#include <sys/wait.h>

#include "daemon/printk.h"
#include "services/services.h"

static int mount_fs(
    const char *source,
    const char *target,
    const char *fstype)
{
    if (mount(source, target, fstype, 0, NULL) == -1) {
        perror(target);
        return 0;
    }

    return 1;
}

int main(void)
{
    printk(NO, MOUNTING, "Device File System...");
    int fs_devtmpfs = mount_fs("devtmpfs","/dev","devtmpfs");

    printk(NO, MOUNTING, "Process File System...");
    int fs_proc = mount_fs("proc","/proc","proc");
    
    printk(NO,MOUNTING, "System Hardware File System...");
    int fs_sys = mount_fs("sysfs","/sys","sysfs");

    printk(fs_devtmpfs, MOUNTED,"Device File System.");
    printk(fs_proc, MOUNTED, "Process File System.");
    printk(fs_sys, MOUNTED, "System Hardware File System.");

    setenv("PATH","/bin",1);

    services_start();

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
