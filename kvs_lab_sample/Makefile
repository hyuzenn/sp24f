# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = 

# Targets
TARGET = kvs
TEST_TARGET = do_baseline

# Source files
SRC = main.c put.c get.c open.c close.c iterator.c
HEADERS = kvs.h
TEST_SRC = do_baseline.c
DATA_FILE = cluster004.trc

# Object files
OBJ = $(SRC:.c=.o)
TEST_OBJ = $(TEST_SRC:.c=.o)

# Default target
all: $(TARGET) $(TEST_TARGET)

# Build KVS executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Build test workload generator
$(TEST_TARGET): $(TEST_OBJ) $(OBJ)  # include all necessary object files
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Run test workload and create snapshot
snapshot: $(TEST_TARGET) $(TARGET)
	@echo "Running test workload to prepare in-memory dataset..."
	./$(TEST_TARGET) $(DATA_FILE)
	@echo "Creating snapshot using do_snapshot..."
	./$(TARGET) do_snapshot kvs.img
	
# Output files
SNAPSHOT_IMG = kvs.img

# Recovery process
recover: $(TARGET)
	@echo "Stopping in-memory KVS..."
	@echo "Recovering KVS from snapshot..."
	./$(TARGET) do_recovery kvs.img

# Clean build artifacts
clean:
	rm -f $(OBJ) $(TEST_OBJ) $(TARGET) $(TEST_TARGET) kvs.img

# Phony targets
.PHONY: all snapshot recover clean
