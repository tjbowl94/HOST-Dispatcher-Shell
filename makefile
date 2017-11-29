# HOST Dispatcher Shell
# By Diego Asis, Jeremiah Bolin, Thomas Bowlin
# CSC400 - Operating Systems, Dr. Zhang
# Fall 2017
hostd: main.c process.c memory.c process.h memory.h
	gcc -o hostd main.c process.c memory.c