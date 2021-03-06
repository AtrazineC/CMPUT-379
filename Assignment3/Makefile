CC			:= g++ -std=c++2a
CFLAGS 		:= -Wall -O
CPPFLAGS 	:= -I src/Common

SERVER_TARGET 	:= server
CLIENT_TARGET 	:= client
SERVER_MAN_TARGET := server.pdf
CLIENT_MAN_TARGET := client.pdf
COMPRESS_TARGET := a3.tar

MAN_DIR	:= docs
SRC_DIR := src
OBJ_DIR := obj

SERVER_SRC_DIR 	:= $(SRC_DIR)/Server
CLIENT_SRC_DIR 	:= $(SRC_DIR)/Client
COMMON_SRC_DIR 	:= ${SRC_DIR}/Common

SERVER_OBJ_DIR := $(OBJ_DIR)/Server
CLIENT_OBJ_DIR := $(OBJ_DIR)/Client
COMMON_OBJ_DIR := $(OBJ_DIR)/Common

SERVER_SRC := $(wildcard $(SERVER_SRC_DIR)/*.cpp)
CLIENT_SRC := $(wildcard $(CLIENT_SRC_DIR)/*.cpp)
COMMON_SRC := $(wildcard $(COMMON_SRC_DIR)/*.cpp)

SERVER_OBJ := $(SERVER_SRC:$(SERVER_SRC_DIR)/%.cpp=$(SERVER_OBJ_DIR)/%.o)
CLIENT_OBJ := $(CLIENT_SRC:$(CLIENT_SRC_DIR)/%.cpp=$(CLIENT_OBJ_DIR)/%.o)
COMMON_OBJ := $(COMMON_SRC:$(COMMON_SRC_DIR)/%.cpp=$(COMMON_OBJ_DIR)/%.o)

.PHONY: all clean

all: $(SERVER_TARGET) $(CLIENT_TARGET)
man: $(SERVER_MAN_TARGET) $(CLIENT_MAN_TARGET)

debug: CFLAGS += -DDEBUG -g
debug: all

$(SERVER_MAN_TARGET):
	groff -Tpdf -man $(MAN_DIR)/server.man > $(SERVER_MAN_TARGET)

$(CLIENT_MAN_TARGET):
	groff -Tpdf -man $(MAN_DIR)/client.man > $(CLIENT_MAN_TARGET)

$(SERVER_TARGET): $(SERVER_OBJ) $(COMMON_OBJ)
	$(CC) $^ -o $@

$(CLIENT_TARGET): $(CLIENT_OBJ) $(COMMON_OBJ)
	$(CC) $^ -o $@

$(SERVER_OBJ_DIR)/%.o: $(SERVER_SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(CLIENT_OBJ_DIR)/%.o: $(CLIENT_SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(COMMON_OBJ_DIR)/%.o: $(COMMON_SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

compress:
	tar -cvf $(COMPRESS_TARGET) Makefile README $(MAN_DIR) $(SRC_DIR)

clean:
	-rm -f -r $(SERVER_MAN_TARGET) $(CLIENT_MAN_TARGET) $(SERVER_TARGET) $(CLIENT_TARGET) $(OBJ_DIR) $(COMPRESS_TARGET)
