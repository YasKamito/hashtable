#
#	make -f <filename>
#

CC=gcc
APPHOME=..
APPOBJ=../object

APPINC   =   ../include
INCDIR   = -I${APPINC}

#CFLAGS=-g -c  -DDEBUG -DDEBUG2 -fprofile-arcs -ftest-coverage
#CFLAGS=-g -c  -DDEBUG -fprofile-arcs -ftest-coverage
CFLAGS=-g -c -fprofile-arcs -ftest-coverage

MODULE=hash

$(APPOBJ)/$(MODULE).o:$(MODULE).c
	$(CC) $(CFLAGS) -c $(MODULE).c ${INCDIR} -o $(APPOBJ)/$(MODULE).o

