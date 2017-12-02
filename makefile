# HOST Dispatcher Shell
# By Diego Asis, Jeremiah Bolin, Thomas Bowlin
# CSC400 - Operating Systems, Dr. Zhang
# Fall 2017
hostd: hostd.c process.c memory.c hostd.h process.h memory.h
	gcc -o hostd hostd.c process.c memory.c