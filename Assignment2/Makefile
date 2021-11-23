CC		:= g++
CFLAGS	:= -Wall -O2
LFLAGS 	:= -pthread

TARGET = prodcon
COMPRESS_TARGET = prodcon.zip

SRC_DIR := src
OBJ_DIR := obj

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

compress:
	zip -r $(COMPRESS_TARGET) Makefile $(SRC_DIR)

clean:
	-rm -f -r $(TARGET) $(OBJ_DIR) $(COMPRESS_TARGET)
