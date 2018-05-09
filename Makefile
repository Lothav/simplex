TARGET = ./simplex
LIBS =
CC = g++

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = src/*.cpp $(wildcard src/**/*.cpp)
HEADERS = src/*.hpp $(wildcard src/**/*.hpp)

%.o: %.c $(HEADERS)
	$(CC)  $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
