CC = gcc
CFLAGS = -Wall -O2 -Iinclude

TARGET = uart_i2c_loopback
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj

# Create obj directory if it doesn't exist
$(shell mkdir -p $(OBJ_DIR))

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(TARGET)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TARGET) $(OBJ_DIR)/*.o
	rmdir $(OBJ_DIR) 2>/dev/null || true

.PHONY: all clean
