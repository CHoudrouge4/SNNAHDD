CC = g++
CFLAGS = -g -Wall
C = -c
VERSION = -std=c++14
OUTPUT  = client
SOURCES = main.cpp rkd_tree.cpp kd_tree.cpp pk_tree.cpp tree.cpp
OBJECTS = $(SOURCES:.cpp=.o)
REMOVE  = client  *.o

$(OUTPUT): $(OBJECTS)
	$(CC) $(CFLAGS) $(VERSION) $(OBJECTS) -o $(OUTPUT)
main.o: main.cpp rkd_tree.cpp tree.cpp rkd_tree.h node.h tree.h
	$(CC) $(VERSION) $(C) $(CFLAGS) main.cpp
tree.o: tree.cpp tree.h node.h
	$(CC) $(VERSION) $(C) $(CFLAGS) tree.cpp
kd_tree.o: kd_tree.cpp kd_tree.h node.h tree.h
	$(CC) $(VERSION) $(C) $(CFLAGS) kd_tree.cpp
rkd_tree.o: rkd_tree.cpp rkd_tree.h node.h tree.h
	$(CC) $(VERSION) $(C) $(CFLAGS) rkd_tree.cpp
pk_tree.o: pk_tree.cpp pk_tree.h node.h tree.h
	$(CC) $(VERSION) $(C) $(CFLAGS) pk_tree.cpp
.PHONY: clean
clean:
	@rm -rf $(REMOVE)
