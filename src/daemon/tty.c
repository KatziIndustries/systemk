// SPDX-License-Identifier: GPL-3.0
/* tty.c
 *
 * gives the user a new tty
 *
 * Author:
 * JRBlockkop <jrblockkop@gmail.com>
*/


#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/wait.h>

void spawn_tty(void)
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        int fd = open("/dev/tty1", O_RDWR);

        if (fd < 0) {
            perror("open /dev/tty1");
            _exit(1);
        }

        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);

        if (fd > STDERR_FILENO)
            close(fd);

        execl("/bin/ksh", "ksh", (char *)NULL);

        perror("exec ksh");
        _exit(127);
    }

    waitpid(pid, NULL, 0);
}
