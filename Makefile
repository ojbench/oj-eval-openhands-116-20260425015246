
CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall -Wextra -Werror

all: test

test: src.hpp
	$(CXX) $(CXXFLAGS) -I. /workspace/data/116/GreedySnakeDown/Game.cpp -o game

run: test
	./game < test.in

clean:
	rm -f game

.PHONY: all test run clean

