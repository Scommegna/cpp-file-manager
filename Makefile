CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -pthread -O3
SANITIZE_FLAGS = -Wall -Wextra -std=c++17 -pthread -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer
THREAD_SANITIZE_FLAGS = -Wall -Wextra -std=c++17 -pthread -O1 -g -fsanitize=thread -fno-omit-frame-pointer

SRC = main.cpp filesystem.cpp utils.cpp tree.cpp search.cpp benchmark.cpp
OUT = fm
SANITIZE_OUT = fm-sanitize
THREAD_SANITIZE_OUT = fm-thread-sanitize

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

sanitize:
	$(CXX) $(SANITIZE_FLAGS) $(SRC) -o $(SANITIZE_OUT)

thread-sanitize:
	$(CXX) $(THREAD_SANITIZE_FLAGS) $(SRC) -o $(THREAD_SANITIZE_OUT)

clean:
	rm -f $(OUT) $(SANITIZE_OUT) $(THREAD_SANITIZE_OUT)

.PHONY: all sanitize thread-sanitize clean
