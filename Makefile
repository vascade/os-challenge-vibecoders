CC = gcc
CFLAGS = -O3 -Wall -Wextra -Iinclude

TARGET = bin/server
SRC = $(wildcard src/*.c)

all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o $@

clean:
	rm -f $(TARGET)

.PHONY: all clean