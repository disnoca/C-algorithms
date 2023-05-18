CC = gcc
CFLAGS = -Wall -Wextra

SOURCES = \
../../../wrapper_functions.c \
avl_tree.c

TARGET = avl_tree_tester

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(TARGET).c $(SOURCES:.c=.o)
	$(CC) $(CFLAGS) -o $@ $< $(SOURCES:.c=.o) 

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(SOURCES:.c=.o) $(TARGET)