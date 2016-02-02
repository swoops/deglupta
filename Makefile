CC = gcc

CFLAGS = -Wall

TARGET = mutator
LIBRARIES = 

all: $(TARGET)

$(TARGET): main.c
	$(CC) -g $(CFLAGS) $(LIBRARIES) -o $(TARGET) main.c

install: $(TARGET)
	install -o root -g root -m 755 $(TARGET) /opt/$(TARGET)

uninstall: 
	rm /opt/$(TARGET)

clean: 
	rm $(TARGET)

