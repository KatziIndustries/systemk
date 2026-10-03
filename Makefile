# SPDX-License-Identifier: GPL-3.0
VERSION = 0
PATCHLEVEL = 1

#Compiler Options
CC = gcc
CC_FLAGS = -static -O2 -Wall -Wextra

all:
	mkdir -p build
	make daemon
	make cli

daemon:
	mkdir -p build/daemon
	$(CC) $(CC_FLAGS) -c src/daemon/*.c -o build/daemon/daemon.o
	$(CC) $(CC_FLAGS) -c src/services/*.c -o build/daemon/services.o

	$(CC) build/daemon/*.o -o build/systemk

cli:
	mkdir -p build/cli
	$(CC) $(CC_FLAGS) -c src/cli/*.c -o build/cli/cli.o

	$(CC) build/cli/*.o -o build/katzictl
