SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build

CC := clang
CCFLAGS := -Wall -Wextra -I$(INCLUDE_DIR)

SRCS := $(SRC_DIR)/main.c
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
TARGET := $(BUILD_DIR)/cppparser

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CCFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir $(dir $@)
	$(CC) $(CCFLAGS) -c -o $@ $<

clean: 
	rm -rf $(BUILD_DIR)

