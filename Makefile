CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -pthread

SRC = src/main.cpp src/server.cpp

TARGET = redis

all:
	$(CXX) $(SRC) $(CXXFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)