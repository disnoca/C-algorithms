CC = gcc
CFLAGS = -Wall -Wextra

FILE = hash_map
TARGET = $(FILE)_tester

all: $(TARGET)

$(FILE).o: $(FILE).c $(FILE).h
	$(CC) $(CFLAGS) -c $(FILE).c

$(TARGET): $(TARGET).c $(FILE).o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(FILE).o ../../../wrapper_functions.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(FILE) $(TARGET)