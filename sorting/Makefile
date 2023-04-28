CC = gcc
CFLAGS = -Wall -Wextra

SOURCES := \
../wrapper_functions.c \
insertion_sort/insertion_sort.c \
bubble_sort/bubble_sort.c \
merge_sort/merge_sort.c

TARGET = sorting_tester

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(TARGET).c $(SOURCES:.c=.o)
	$(CC) $(CFLAGS) -o $@ $< $(SOURCES:.c=.o) 

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(SOURCES) $(TARGET)