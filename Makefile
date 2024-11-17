CC=g++
CFLAGS= -Wall -Wextra -std=c++20 -I./includes
EXECUTABLE=emulator
INCLUDES=./includes
SRC=./src
OBJECTS=*.o

.PHONY: clean

$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ -o $@


%.o: emulator.cpp $(SRC)/%.cpp
	$(CC) -c $(CFLAGS) $^


clean:
	rm *.o $(EXECUTABLE)