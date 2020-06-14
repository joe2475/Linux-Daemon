#!/usr/bin/make -f
CC = g++
CPPFLAGS = -Wall

TFLAGS = -I
TDIR = ~/include/include/
PROJECTNAME = Program5
LDLIBS = -lrudeconfig
LDFLAGS = -static -L/home/012/j/jt/jtb170930/include/rude/lib/ 
RUDEDIR = ~/include/rude/include/
EXECFILE = program5

OBJS = program5.cpp demon.cpp


all: $(EXECFILE)

$(EXECFILE):	$(OBJS)
	$(CC) $(CPPFLAGS) -static -o $@ $(OBJS) $(TFLAGS) $(TDIR) $(TFLAGS) $(RUDEDIR)  $(LDFLAGS) $(LDLIBS)



