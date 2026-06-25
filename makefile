CXX = g++
CXXFLAGS = -std=c++17 -Wall

all:
	$(CXX) $(CXXFLAGS) main.cpp -o dedup

clean:
	rm -f dedup storage.dat