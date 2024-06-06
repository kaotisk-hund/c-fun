CC=gcc
CFLAGS=-Wall -Wextra -Werror -Wshadow -pedantic -ggdb -lm -std=c11 -I./include
FILES=src/account.c src/offer.c src/market-cit.c src/main.c 
TARGET=market-understanding
market-understanding:
	$(CC) $(CFLAGS) $(FILES) -o bin/$(TARGET)

tests:
	$(CC) $(CFLAGS) $(FILES) -o bin/$(TARGET)

clean:
	rm bin/$(TARGET)
