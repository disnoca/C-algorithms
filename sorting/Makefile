CC = gcc
CFLAGS = -Wall -Wextra -Wno-unused-function

SOURCES := \
../wrapper_functions.c \
helper_functions.c \
insertion_sort.c \
bubble_sort.c \
selection_sort.c \
merge_sort.c \
in_place_merge_sort.c \
heapsort.c \
quicksort.c \
introsort.c \

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