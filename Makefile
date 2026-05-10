SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build

ifeq ($(OS),Windows_NT)
    MKDIR = if not exist "$@" mkdir
    RM = rmdir /s /q
else
    MKDIR = mkdir -p
    RM = rm -rf
endif

CC := clang
CCFLAGS := -Wall -Wextra -I$(INCLUDE_DIR)

SRCS := $(SRC_DIR)/main.c $(SRC_DIR)/lexer.c
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
TARGET := $(BUILD_DIR)/cppparser

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CCFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(MKDIR) $(dir $@)
	$(CC) $(CCFLAGS) -c -o $@ $<

clean: 
	$(RM) $(BUILD_DIR)

