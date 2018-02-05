CC=gcc
LDFLAGS=
CFLAGS =-W -Wall -ansi -pedantic

DIR=.
BIN=$(DIR)/bin/
OBJ=$(DIR)/obj/
SRC=$(DIR)/src/

EXEC= $(BIN)graph $(BIN)random

all: $(EXEC)

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

clean:
	rm $(OBJ)*.o $(BIN)*
