CC = gcc
CFLAGS = -O3 -Wall -Wextra -Iinclude
 
TARGET = server
SRC = $(wildcard src/*.c)
 
all: $(TARGET)
 
$(TARGET): $(SRC) $(wildcard include/*.h)
	$(CC) $(CFLAGS) $(SRC) -o $@
 
clean:
	rm -f $(TARGET)
 
.PHONY: all clean