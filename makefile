# HOST Dispatcher Shell
# By Diego Asis, Jeremiah Bolin, Thomas Bowlin
# CSC400 - Operating Systems, Dr. Zhang
# Fall 2017
all: hostd process

hostd: hostd.c process.c memory.c resources.c hostd.h process.h memory.h resources.h
	gcc -o hostd hostd.c process.c memory.c resources.c

process: sigtrap.c
	gcc -o process sigtrap.c
