# Compiler
CC = gcc

# Target executable
TARGET = miris

# Source files
SRC = miris.c modules/utils.c modules/ADTGraph.c modules/ADTHash.c modules/myAllocation.c

# Header files
HEADERS = include/ADTGraph.h include/ADTHash.h include/utils.h include/myAllocation.h

# Include directories
INCLUDES = -Iinclude

# Build target
$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(INCLUDES) -o $(TARGET) $(SRC)

# Run target
run: $(TARGET)
	./$(TARGET)

# Valgrind target
valgrind: $(TARGET)
	valgrind -s --leak-check=full ./$(TARGET)

# Clean target
clean:
	rm -f $(TARGET)