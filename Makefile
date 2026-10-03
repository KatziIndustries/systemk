# SPDX-License-Identifier: GPL-3.0
VERSION = 0
PATCHLEVEL = 1

#Compiler Options
CC = gcc
CFLAGS = -Isrc -static -O2 -Wall -Wextra

#Source Code Files
DAEMON_SRC := $(wildcard src/daemon/*.c)
DAEMON_OBJ := $(patsubst src/daemon/%.c,build/daemon/%.o,$(DAEMON_SRC))
CLI_SRC := $(wildcard src/cli/*.c)
CLI_OBJ := $(patsubst src/cli/%.c,build/cli/%.o,$(CLI_SRC))

.PHONY: all daemon cli clean

all: daemon cli

daemon: $(DAEMON_OBJ)
	$(CC) $^ -o build/systemk

cli: $(CLI_OBJ)
	$(CC) $^ -o build/katzictl

clean:
	rm -fr build

#Daemon Pattern Rule
build/daemon/%.o: src/daemon/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

#Cli Pattern Rule
build/cli/%.o: src/cli/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@