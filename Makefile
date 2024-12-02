# Compiler
CC := gcc

# Compiler Flags
CFLAGS := -w -Wextra -pthread -lm  # Thêm -lm vào CFLAGS

# Directories
CLIENT_DIR := client_side
SERVER_DIR := server_side

# Source Files
CLIENT_SRCS := $(wildcard $(CLIENT_DIR)/*.c)
SERVER_SRCS := $(wildcard $(SERVER_DIR)/*.c)

# Object Files
CLIENT_OBJS := $(CLIENT_SRCS:.c=.o)
SERVER_OBJS := $(SERVER_SRCS:.c=.o)

# Executable Names
CLIENT_EXEC := client
SERVER_EXEC := server

# Default Target
all: $(CLIENT_EXEC) $(SERVER_EXEC)

# Build Client Executable
$(CLIENT_EXEC): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Build Server Executable
$(SERVER_EXEC): $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile Source Files into Object Files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean Up
clean:
	rm -f $(CLIENT_OBJS) $(SERVER_OBJS) $(CLIENT_EXEC) $(SERVER_EXEC)

# Phony Targets
.PHONY: all clean
