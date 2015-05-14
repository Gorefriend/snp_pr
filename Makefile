#!/usr/bin/make -f
#
# Project Makefile
#
# Author: Philip Muskovac

CFLAGS := -Wall -Wextra -g
CC = gcc $(CFLAGS)

# all application binaries
all: logger.exe

# test binaries
test: all
	$(CC) log_test.c -o log_test.exe

clean:
	rm -f *.o *.exe
	
logger.exe: logger.c logger.h
	$(CC) logger.c -o logger.exe

logger_client.o: logger_client.* logger.h
	$(CC) logger_client.* logger.h -c -o logger_client.o
