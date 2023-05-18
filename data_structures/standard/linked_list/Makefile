CC = gcc
CFLAGS = -Wall -Wextra

SOURCES = \
../../../wrapper_functions.c \
linked_list.c

TARGET = linked_list_tester

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(TARGET).c $(SOURCES:.c=.o)
	$(CC) $(CFLAGS) -o $@ $< $(SOURCES:.c=.o) 

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(SOURCES:.c=.o) $(TARGET)