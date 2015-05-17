#!/usr/bin/make -f
#
# Project Makefile
#
# Author: Philip Muskovac

CFLAGS := -Wall -g #-Wextra
CC = gcc $(CFLAGS)

# all application binaries
all: logger.exe

# test binaries
test: all logger_client.o
	$(CC) log_test.c logger_client.o -o log_test.exe

clean:
	rm -f *.o *.exe
	
logger.exe: logger_client.o logger.c logger.h
	$(CC) logger.c logger_client.o -o logger.exe

logger_client.o: logger_client.[ch] logger.h
	$(CC) -c logger_client.c -o logger_client.o
