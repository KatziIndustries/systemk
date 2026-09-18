#ifndef PRINTK_H
#define PRINTK_H

#define FAIL 0
#define OK 1
#define NO 2

#define REACHED "Reached"
#define LISTENING "Listening"
#define MOUNTING "Mounting"
#define MOUNTED "Mounted" 
#define STARTING "Starting"
#define STARTED "Started" 
#define CREATED "Created"


void printk(int status, char *action, char *str);

#endif