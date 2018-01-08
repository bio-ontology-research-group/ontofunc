#

CXX=g++
CXXFLAGS=-g -O2 
LIBS=-lRmath 
LDFLAGS=

install=/usr/bin/install

prefix = /home/leechuck
exec_prefix = ${prefix}
bindir = ${exec_prefix}/bin
libdir = ${exec_prefix}/lib

OBJS = transitions.o idmap.o overall_sign.o

all: $(OBJS)

%.o: %.cc %.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS)

dist-clean: clean
	rm -f Makefile

install:
