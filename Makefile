CC = g++
CFLAGS = -g -Wall
C = -c
VERSION = --std=c++14
OUTPUT  = client
SOURCES = main.cpp kd_tree.cpp
OBJECTS = $(SOURCES:.cpp=.o)
REMOVE  = client  *.o

$(OUTPUT): $(OBJECTS)
	$(CC) $(CFLAGS) $(VERSION) $(OBJECTS) -o $(OUTPUT) 
main.o: main.cpp kd_tree.cpp kd_tree.h
	$(CC) $(VERSION) $(C) $(CFLAGS) main.cpp 
kd_tree.o: kd_tree.h
	$(CC) $(VERSION) $(C) $(CFLAGS) kd_tree.cpp
.PHONY: clean
clean:
	@rm -rf $(REMOVE)
