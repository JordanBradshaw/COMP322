#Jordan Bradshaw
#lab7

EXECUTABLE-L=matrix_add
EXECUTABLE-T=matrix_gen
SRC-L=matrix_add.c
SRC-T=matrix_gen.c
OBJ-L=matrix_add.o
OBJ-T=matrix_gen.o

CC=gcc
CFLAGS=-g

all: $(EXECUTABLE-L) $(EXECUTABLE-T)


$(EXECUTABLE-L): $(SRC-L)
	$(CC) $(CFLAGS) -c $(SRC-L)
	$(CC) $(CFLAGS) -o $@ $(OBJ-L)

$(EXECUTABLE-T): $(SRC-T)
	$(CC) $(CFLAGS) -c $(SRC-T)
	$(CC) $(CFLAGS) -o $@ $(OBJ-T)

clean:
	rm -f $(EXECUTABLE-L) $(EXECUTABLE-T) $(OBJ-L) $(OBJ-T) *~
