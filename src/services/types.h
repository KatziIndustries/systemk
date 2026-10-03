// SPDX-License-Identifier: GPL-3.0
/* types.h
 *
 * the types that are used in the
 * service submodule of systemk
 *
 * Author:
 * JRBlockkop <jrblockkop@gmail.com>
*/

#ifndef TYPES_H
#define TYPES_H

#define SYSTEMD_DIR "/etc/systemd/system"

#include <stddef.h>
#include <sys/types.h>

typedef struct {
    char **items;
    size_t count;
    size_t capacity;
} StringArray;

typedef struct {
    char *name;
    char *path;

    char *description;
    StringArray after;

    char *exec_start;
    int bg;
} ServiceUnit;

typedef struct {
    ServiceUnit *items;
    size_t count;
    size_t capacity;
} ServiceArray;

#endif