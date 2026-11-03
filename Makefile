CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

SRC = src/main.c src/vm.c src/loader.c
OBJ = $(SRC:.c=.o)
TARGET = qezvm

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean