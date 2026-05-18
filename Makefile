SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build

ifeq ($(OS),Windows_NT)
MKDIR = if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
RM = rmdir /s /q 
TARGET = $(BUILD_DIR)/cppparser.exe
else
MKDIR = mkdir -p $(BUILD_DIR)
RM = rm -rf
TARGET := $(BUILD_DIR)/cppparser
endif

CC := clang
CCFLAGS := -Wall -Wextra -I$(INCLUDE_DIR)

SRCS := $(SRC_DIR)/main.c $(SRC_DIR)/lexer.c $(SRC_DIR)/parser.c $(SRC_DIR)/semantic.c
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CCFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(MKDIR) 
	$(CC) $(CCFLAGS) -c -o $@ $<

clean: 
	$(RM) $(BUILD_DIR)

