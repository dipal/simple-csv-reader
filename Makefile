CXX=g++
CXXFLAGS=-g -std=c++11 -Wall -pedantic
BIN=csv_reader_test

SRC=csv_reader_test.cpp
OBJ=$(SRC:%.cpp=%.o)

all: $(OBJ)
	$(CXX) -o $(BIN) $^

%.o: %.c
	$(CXX) $@ -c $<

clean:
	rm -f *.o
	rm $(BIN)


test:
	./csv_reader_test
