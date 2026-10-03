// SPDX-License-Identifier: GPL-3.0
/* printk.c
 *
 * Logger
 *
 * Author:
 * JRBlockkop <jrblockkop@gmail.com>
*/

#include <stdio.h>

#include "daemon/printk.h"

void printk(int status, char *action, char *str)
{
    if(status == OK) {
        printf("[  \x1B[32mOK\x1B[0m  ] ");
    } else if(status == FAIL) {
        printf("[\x1B[31mFAILED\x1B[0m] ");
    } else {
        printf("         ");
    }
    printf("%s %s\n",action,str);
}