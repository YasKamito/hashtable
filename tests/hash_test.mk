CC            = g++
GTEST_INCPATH = ../gtest/include
GTEST_LIBPATH = ../gtest/lib

SRC_OBJPATH   = ../object
SRC_INCPATH   = ../include
SRC_LIBPATH   = /usr/local/bin

TEST_BINPATH  = .
TEST_OBJPATH  = .
TEST_SRCPATH  = .

MODULE        = hash_test
EXECUTE       = $(TEST_BINPATH)/$(MODULE)

CFLAGS        = -g -c
LFLAGS        = -lgtest_main -lgcov -fprofile-arcs

INCS=-I$(GTEST_INCPATH) -I$(SRC_INCPATH)
LIBS=-L$(SRC_LIBPATH) -L$(GTEST_LIBPATH)
SRCOBJS=$(SRC_OBJPATH)/hash.o 

#--- linkage ---#
$(EXECUTE) : $(TEST_OBJPATH)/$(MODULE).o $(SRCOBJS)
	$(CC) $(LIBS) $(LFLAGS) -o $(EXECUTE) $(SRCOBJS) $(TEST_OBJPATH)/$(MODULE).o

#--- compile ---#
$(TEST_OBJPATH)/$(MODULE).o : $(TEST_SRCPATH)/$(MODULE).cpp
	$(CC) $(CFLAGS) $(TEST_SRCPATH)/$(MODULE).cpp -o $(TEST_OBJPATH)/$(MODULE).o $(INCS)
