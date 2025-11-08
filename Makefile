CC := gcc
CFLAGS := -Wall -Iinclude -Werror -g
TARGET := basic_server.exe
SRC_DIR:=src
OBJ_DIR := build

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(TARGET)
	@echo "build complete: $(TARGET)"
	@echo "cleaning up object files"
	@rm -rf $(OBJ_DIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -f $(OBJ_DIR) $(TARGET)

# .PHONY: all clean
