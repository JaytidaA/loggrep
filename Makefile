CC := gcc
CFLAGS := -O2 -W -Wall -Wextra -pedantic -std=c99

SRC_DIR := src
INC_DIR := include
BIN_DIR := bin
BLD_DIR := build

SRC := main.c filters.c options.c utils.c colours.c
OBJ := $(SRC:%.c=$(BIN_DIR)/%.o)

.PHONY: all clean clean_all

all: $(BLD_DIR)/loggrep

$(BLD_DIR)/loggrep: $(OBJ) | $(BLD_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

$(BIN_DIR)/main.o: main.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BLD_DIR):
	mkdir -p $(BLD_DIR)

clean:
	rm -f $(BIN_DIR)/*.o

clean_all:
	rm -rf $(BIN_DIR) $(BLD_DIR)

