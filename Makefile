CC = gcc
CFLAGS = -Wall -Iinclude -Werror -g
TARGET = basic_server.exe
OBJS = src/main.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

src/main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

clean:
	rm -f src/*.o $(TARGET)

# .PHONY: all clean
