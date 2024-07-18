CC = gcc
CFLAGS = -Wall -Wextra -g

SOURCES = ../../wrapper_functions.c graph.c

TARGET = graph_tester

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(TARGET).c $(SOURCES:.c=.o)
	$(CC) $(CFLAGS) -o $@ $< $(SOURCES:.c=.o) 

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(SOURCES:.c=.o) $(TARGET)
