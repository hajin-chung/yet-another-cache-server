CC = gcc
CFLAGS = -Wall -Wextra -Werror
OBJ_DIR = obj
BIN_DIR = bin
SERVER_SRC = src/server.c src/hash.c src/logger.c src/request.c src/response.c
CLIENT_SRC = src/client.c src/hash.c src/logger.c src/request.c src/response.c
SERVER_OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(SERVER_SRC:.c=.o)))
CLIENT_OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(CLIENT_SRC:.c=.o)))

all: server client

server: $(SERVER_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/server $(SERVER_OBJ)

client: $(CLIENT_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/client $(CLIENT_OBJ)

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)/server $(BIN_DIR)/client

.PHONY: all server client clean
