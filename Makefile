CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -O2
INCLUDES = -Iinclude -Ilibs

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

TARGET = $(BIN_DIR)/main

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

.PHONY: all run test clean

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	./constraints.sh ./$(TARGET) 65536

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
