CXX = g++ -std=c++23
CFLAGS = -Wall -Wextra -pedantic
LIBS = -lraylib -lm

main: main.cpp
	$(CXX) $(CFLAGS) main.cpp -o main $(LIBS)
