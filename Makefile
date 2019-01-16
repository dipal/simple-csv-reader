CXX=g++
CXXFLAGS=-g -std=c++11 -Wall -pedantic
BIN=csv_reader

SRC=csv_reader.cpp
OBJ=$(SRC:%.cpp=%.o)

all: $(OBJ)
	$(CXX) -o $(BIN) $^

%.o: %.c
	$(CXX) $@ -c $<

clean:
	rm -f *.o
	rm $(BIN)


test:
	./csv_reader
