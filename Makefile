CXX = g++
CXXFLAGS = -Wall -std=c++17

SRC = main.cpp filesystem.cpp utils.cpp
OUT = fm

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)