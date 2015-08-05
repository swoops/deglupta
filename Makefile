CC = gcc

CFLAGS = -Wall

TARGET = mutator
LIBRARIES = -lconfig

all: $(TARGET)

$(TARGET): main.c
	$(CC) -g $(CFLAGS) $(LIBRARIES) -o $(TARGET) main.c

clean: 
	rm $(TARGET)

