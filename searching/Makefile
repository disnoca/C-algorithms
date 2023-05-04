CC = gcc
CFLAGS = -Wall -Wextra

SOURCES := \
../wrapper_functions.c \
linear_search.c \
binary_search.c

TARGET = searching_tester

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(TARGET).c $(SOURCES:.c=.o)
	$(CC) $(CFLAGS) -o $@ $< $(SOURCES:.c=.o) 

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(SOURCES) $(TARGET)