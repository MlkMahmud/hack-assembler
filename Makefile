TARGET = main

CC = gcc

CFLAGS = -Wall -Wextra -g

SOURCES = src/main.c src/parser.c src/tables.c src/utils.c

OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: clean