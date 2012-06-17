CC=g++
CFLAGS= -lGLU -lglut -lSDL -lSDL_image
LDFLAGS=
SOURCES=demo.cpp
OBJECTS=$(SOURCES)
EXECUTABLE=demo

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
 
