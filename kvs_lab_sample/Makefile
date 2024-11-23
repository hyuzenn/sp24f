# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = 

# Targets
TARGET = kvs
TEST_TARGET = do_baseline

# Source files
SRC = main.c put.c get.c open.c close.c iterator.c do_baseline.c
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

# Run test workload, create snapshot, and recover KVS
snapshot_and_recover: $(TARGET)
	@echo "Running test workload to prepare in-memory dataset..."
	./$(TARGET)
	@echo "Snapshot created and recovery done successfully."

# Output files
SNAPSHOT_IMG = kvs.img

# Clean build artifacts
clean:
	rm -f $(OBJ) $(TEST_OBJ) $(TARGET) $(TEST_TARGET) kvs.img answer.dat

# Phony targets
.PHONY: all snapshot_and_recover clean
