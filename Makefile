
# Paths
DSRC = src
DINC = inc
DOBJ = obj
DEXE = exe
DTST = tests

# Compiler flags
CFLAGS = -I$(DINC) -Wall -g

# Commands
CC = gcc $(CFLAGS)
LK = ld
RM = rm

# Objects

$(DOBJ)/amatrix.obj: $(DSRC)/amatrix.c $(DINC)/amatrix.h
	$(CC) -o $(DOBJ)/amatrix.obj -c $(DSRC)/amatrix.c
	
$(DOBJ)/utils.obj: $(DSRC)/utils.c $(DINC)/utils.h
	$(CC) -o $(DOBJ)/utils.obj -c $(DSRC)/utils.c
	
$(DOBJ)/tGraph.obj: $(DSRC)/tGraph.c $(DINC)/tGraph.h
	$(CC) -o $(DOBJ)/tGraph.obj -c $(DSRC)/tGraph.c

# naive algo
$(DOBJ)/nSteiner.obj: $(DSRC)/nSteiner.c $(DINC)/steiner.h
	$(CC) -o $(DOBJ)/nSteiner.obj -c $(DSRC)/nSteiner.c
# algo 1
$(DOBJ)/Steiner1.obj: $(DSRC)/Steiner1.c $(DINC)/steiner.h
	$(CC) -o $(DOBJ)/Steiner1.obj -c $(DSRC)/Steiner1.c
	
$(DOBJ)/enumeration.obj: $(DSRC)/enumeration.c
	$(CC) -o $(DOBJ)/enumeration.obj -c $(DSRC)/enumeration.c 

$(DOBJ)/naive.obj: $(DTST)/naive.c
	$(CC) -o $(DOBJ)/naive.obj -c $(DTST)/naive.c 

$(DOBJ)/enum.obj: $(DTST)/enum.c
	$(CC) -o $(DOBJ)/enum.obj -c $(DTST)/enum.c

$(DOBJ)/algo1.obj: $(DTST)/algo1.c
	$(CC) -o $(DOBJ)/algo1.obj -c $(DTST)/algo1.c


# Execs

all: algo1

naive: $(DOBJ)/amatrix.obj $(DOBJ)/utils.obj $(DOBJ)/tGraph.obj $(DOBJ)/nSteiner.obj $(DOBJ)/naive.obj
	$(CC) -o $(DEXE)/naive  $(DOBJ)/amatrix.obj $(DOBJ)/utils.obj $(DOBJ)/tGraph.obj $(DOBJ)/nSteiner.obj $(DOBJ)/naive.obj

algo1: $(DOBJ)/amatrix.obj $(DOBJ)/utils.obj $(DOBJ)/tGraph.obj $(DOBJ)/Steiner1.obj $(DOBJ)/enumeration.obj $(DOBJ)/algo1.obj
	$(CC) -o $(DEXE)/algo1  $(DOBJ)/amatrix.obj $(DOBJ)/utils.obj $(DOBJ)/tGraph.obj $(DOBJ)/Steiner1.obj $(DOBJ)/enumeration.obj $(DOBJ)/algo1.obj
	
enum: $(DOBJ)/enum.obj $(DOBJ)/amatrix.obj $(DOBJ)/tGraph.obj $(DOBJ)/utils.obj $(DOBJ)/enumeration.obj
	$(CC) -o $(DEXE)/enum  $(DOBJ)/amatrix.obj $(DOBJ)/utils.obj $(DOBJ)/tGraph.obj $(DOBJ)/enum.obj $(DOBJ)/enumeration.obj


# Clean all target
clean:
	$(RM) $(DOBJ)/*.obj
	$(RM) $(DEXE)/*
