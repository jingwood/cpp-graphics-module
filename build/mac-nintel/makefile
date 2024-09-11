CXX=clang++
CXXFLAGS=-O3 -std=c++11 -pthread -Wall #-pedantic
INC=-I../../inc/jpeg-6b -I../../inc/
LIB=
CX=$(CXX) $(CXXFLAGS)
BIN=libugm.a

# Change this to the folder (cpp-common-class, libucm)
INC+=-I../../../cpp-common-class/src

VPATH=../../src/ugm

SRCS = $(wildcard $(VPATH)/*.cpp)
OBJS = $(patsubst %.cpp, %.o, $(notdir $(SRCS)))

all: $(BIN)

$(BIN): $(OBJS)
	ar -crv $(LIB) $(BIN) $(OBJS)

%.o:    %.cpp
	$(CX) $(INC) -c $< -o $@

clean:
	rm -f $(BIN) *.o
	rm -rf $(BIN).dSYM

