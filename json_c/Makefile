CC=gcc
CFLAGS=-Wall -Wextra -Werror -Wshadow -pedantic -ggdb -lm -std=c11 -I./include
FILES=src/casting.c src/akstuff.c src/json_read.c 
TARGET=bin/json_read
json_read:
	$(CC) $(CFLAGS) $(FILES) -o $(TARGET)

clean:
	rm json_read
