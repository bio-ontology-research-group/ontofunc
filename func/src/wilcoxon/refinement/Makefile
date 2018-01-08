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

TARGET = func_wilcoxon_refin
OBJS = gene.o genes.o go_graph.o go_obj.o ../../common/idmap.o ../../common/transitions.o main.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIBS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm -f *.o $(TARGET)

dist-clean: clean
	rm -f Makefile

install:
	$(install) $(TARGET) $(bindir)
