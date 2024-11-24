# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -pg
LDFLAGS = 

# Targets
TARGET = kvs
BASELINE_TARGET = do_baseline
CUSTOM_TARGET = do_custom

# Source files
SRC = main.c put.c get.c open.c close.c iterator.c
BASELINE_SRC = do_baseline.c
CUSTOM_SRC = do_custom.c
HEADERS = kvs.h
DATA_FILE = cluster004.trc

# Object files
OBJ = $(SRC:.c=.o)
BASELINE_OBJ = $(BASELINE_SRC:.c=.o)
CUSTOM_OBJ = $(CUSTOM_SRC:.c=.o)

# Default target
all: $(TARGET) $(BASELINE_TARGET) $(CUSTOM_TARGET)

# Build KVS executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Build baseline workload generator
$(BASELINE_TARGET): $(BASELINE_OBJ) $(OBJ)  # include all necessary object files
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Build custom workload generator
$(CUSTOM_TARGET): $(CUSTOM_OBJ) $(OBJ)  # include all necessary object files
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Run baseline test workload, create snapshot, and recover KVS
baseline: $(BASELINE_TARGET)
	@echo "Running baseline test workload to prepare in-memory dataset..."
	./$(BASELINE_TARGET)
	@echo "Snapshot created and recovery done successfully."

# Run custom test workload, create snapshot, and recover KVS
custom: $(CUSTOM_TARGET)
	@echo "Running custom test workload to prepare in-memory dataset..."
	./$(CUSTOM_TARGET)
	@echo "Snapshot created and recovery done successfully."

# Output files
SNAPSHOT_IMG = kvs.img

# Clean build artifacts
clean:
	rm -f $(OBJ) $(BASELINE_OBJ) $(CUSTOM_OBJ) $(TARGET) $(BASELINE_TARGET) $(CUSTOM_TARGET) kvs.img answer.dat

# Phony targets
.PHONY: all snapshot_and_recover_baseline snapshot_and_recover_custom clean
