CC = gcc
LDFLAGS =
CFLAGS = -W -Wall -ansi -pedantic -g

DIR = .
BIN = $(DIR)/bin/
OBJ = $(DIR)/obj/
SRC = $(DIR)/src/

EXEC = $(BIN)graph $(BIN)random
TEST = $(BIN)test

all: $(EXEC)

test: $(TEST)

$(BIN)test: $(OBJ)utils.o $(OBJ)test.o
	$(CC) -o $(BIN)test $(OBJ)test.o $(OBJ)utils.o $(LDFLAGS)

$(BIN)graph: $(OBJ)utils.o $(OBJ)graph.o
	$(CC) -o $(BIN)graph $(OBJ)graph.o $(OBJ)utils.o $(LDFLAGS)

$(BIN)random: $(OBJ)utils.o $(OBJ)random.o
	$(CC) -o $(BIN)random $(OBJ)random.o $(OBJ)utils.o $(LDFLAGS)

$(OBJ)utils.o: $(SRC)utils.c
	$(CC) -o $(OBJ)utils.o -c $(SRC)utils.c $(CFLAGS)

$(OBJ)graph.o: $(SRC)graph.c $(SRC)utils.h
	$(CC) -o $(OBJ)graph.o -c $(SRC)graph.c $(CFLAGS)

$(OBJ)random.o: $(SRC)random.c $(SRC)utils.h
	$(CC) -o $(OBJ)random.o -c $(SRC)random.c $(CFLAGS)

$(OBJ)test.o: $(SRC)test.c $(SRC)utils.h
	$(CC) -o $(OBJ)test.o -c $(SRC)test.c $(CFLAGS)

clean:
	rm $(OBJ)*.o $(BIN)*
