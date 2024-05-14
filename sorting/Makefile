CC = gcc
CFLAGS = -Wall -Wextra -lm

SOURCES := ../wrapper_functions.c \
insertion_sort.c \
bubble_sort.c \
selection_sort.c \
merge_sort.c \
heap_sort.c \
quick_sort.c \
intro_sort.c

TARGET = sorting_tester

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(TARGET).c $(SOURCES:.c=.o)
	$(CC) -o $@ $< $(SOURCES:.c=.o) $(CFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(SOURCES:.c=.o) $(TARGET)
