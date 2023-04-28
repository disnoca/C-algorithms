CC = gcc
CFLAGS = -Wall -Wextra

FILES :=\
insertion_sort/insertion_sort

TARGET = sorting_tester

all: $(TARGET)

$(FILES).o: $(FILES).c
	$(CC) $(CFLAGS) -c $(FILES).c -o $(FILES).o 

$(TARGET): $(TARGET).c $(FILES).o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(FILES).o ../wrapper_functions.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(FILES) $(TARGET)