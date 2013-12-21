CC=g++
CFLAGS=-c -Wall -O3 -I/usr/local/include/libfreenect
LDFLAGS=-L/usr/local/lib/ -lfreenect
SOURCES=tiltdemo.cpp
OBJECTS=$(SOURCES:%.cpp=%.o)
EXECUTABLE=tiltdemo
 
all: $(SOURCES) $(EXECUTABLE)
 
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
 
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
 
clean :
	rm -f *.o $(EXECUTABLE)
