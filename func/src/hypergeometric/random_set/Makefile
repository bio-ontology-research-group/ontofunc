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

TARGET = func_hyper_randset
OBJS = go_graph.o go_obj.o go.o ../../common/idmap.o ../../common/transitions.o main.o 

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIBS) 

main.o: main.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

%.o: %.cc %.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm -f *.o func_hyper_randset

dist-clean: clean
	rm -f Makefile

install: $(TARGET) 
	$(install) $(TARGET) $(bindir)
