CC = gcc
CFLAGS = -Wall -Wextra -g

SOURCES = fs.c path_utils.c sufs.c disk.c ../wrapper_functions.c ../data_structures/bitmap/bitmap.c

TARGET = fs_tester

all: $(TARGET) newfs disk.img

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(TARGET).c $(SOURCES:.c=.o)
	$(CC) $(CFLAGS) -o $@ $< $(SOURCES:.c=.o)

newfs: newfs.c disk.o ../wrapper_functions.o
	$(CC) $(CFLAGS) -o $@ $< disk.o ../wrapper_functions.o

disk.img:
	dd if=/dev/zero of=disk.img bs=1M count=100

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(SOURCES:.c=.o) $(TARGET)
	rm -f newfs
