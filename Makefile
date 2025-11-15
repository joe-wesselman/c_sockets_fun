CC := gcc


DEBUG ?= 0

ifeq ($(DEBUG),1)
	CFLAGS := -Wall -Iinclude -Werror -g -O0
	TARGET := basic_server_debug.exe
else
	CFLAGS := -Wall -Iinclude -Werror -O2
	TARGET := basic_server.exe
endif

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
	rm -f $(OBJ_DIR) *.exe

# .PHONY: all clean
