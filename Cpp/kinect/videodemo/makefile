CC=g++
CFLAGS=-c -Wall -O3 -I/usr/local/include/libfreenect/ -I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads/
LDFLAGS=-L/usr/local/lib/ -L/opt/vc/lib/ -lfreenect -lbcm_host -lSDL -lSDL_image
SOURCES=tiltdemo.cpp
#DEPENDENCIES=kinect.hpp libfreenect.hpp

OBJECTS=$(SOURCES:%.cpp=%.o)
SRC_DIR=./src/
EXECUTABLE=kinectdemo
 
: $(SRC_DIR)$(SOURCES) $(SRC_DIR)$(EXECUTABLE)

$(EXECUTABLE): $(SRC_DIR)$(OBJECTS)
	$(CC) $(LDFLAGS) $(SRC_DIR)$(OBJECTS) -o $@
 
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
 
clean :
	rm -f *.o $(EXECUTABLE)
