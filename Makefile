CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -pthread

SRC = main.cpp filesystem.cpp utils.cpp tree.cpp search.cpp benchmark.cpp
OUT = fm

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)