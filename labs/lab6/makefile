CC = g++
CFLAGS = -g -Wall

BIN = Mazemake Mazesolve Mazeimage

all: $(BIN)

.cpp: 
	$(CC) $(CFLAGS) -o $* $<

clean:
	@rm -f $(BIN)