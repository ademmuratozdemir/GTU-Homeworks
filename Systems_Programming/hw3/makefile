# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -pthread

# Target executable
TARGET = parking_system

# Source file
SRC = test.c

# Object file
OBJ = $(SRC:.c=.o)

# Default target
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Rule to build object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJ) $(TARGET)

# Run target
run: $(TARGET)
	./$(TARGET)