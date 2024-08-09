# Compiler and flags
CC := clang

# Source files
SRCS := dtb_unpacker.c utils.c

# Object files
OBJS := $(SRCS:.c=.o)

# Executable name
TARGET := dtb_unpacker

# Build rule
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)
