UNAME := $(shell uname)

SHELL = /bin/sh

RELATIVE_ROOT_DIR = ./
TEST_DIR = ./unittests/
BIN_DIR = ./bin/

.SUFFIXES:
.SUFFIXES: .cpp .o

LIB_STD = -std=c++0x
LIB_GRAPHICS_OPENGL = -lglut -lGL -lGLU
LIB_GRAPHICS_OPENCV = -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_ts -lopencv_video -I/usr/include/ni -L/home/kgee/kinect/OpenNI-Bin-Dev-Linux-x64-v1.5.4.0/Lib -lOpenNI

ifeq ($(UNAME), Linux)

LIB_OS = 
LIB_GRAPHICS_SDL = -lSDL -lSDL_image
LIB_PATH =
INCLUDE_PATH = 

endif

ifeq ($(UNAME), CYGWIN_NT-6.1-WOW64)

LIB_OS = -lmingw32 -mwindows
LIB_GRAPHICS_SDL = -lSDLMain -lSDL -lSDL_image -lSDL_gfx
LIB_PATH = -L ./lib
INCLUDE_PATH = -I ./include

endif

game:		game.cpp
	g++ -o $(BIN_DIR)game game.cpp $(LIB_OS) $(LIB_STD) $(LIB_GRAPHICS_SDL) $(LIB_PATH) $(INCLUDE_PATH)

kinect:		KinectDemo.cpp
	g++ -o $(BIN_DIR)kinect KinectDemo.cpp $(LIB_OS) $(LIB_STD) $(LIB_GRAPHICS_SDL) $(LIB_GRAPHICS_OPENGL) $(LIB_GRAPHICS_OPENCV)

sdl :		SDLDemo.cpp
	g++ -o $(BIN_DIR)sdl SDLDemo.cpp $(LIB_OS) $(LIB_STD) $(LIB_GRAPHICS_SDL)
	
ghsom :		ghsom.cpp
	g++ -o $(BIN_DIR)ghsom ghsom.cpp $(LIB_OS) $(LIB_STD) $(LIB_GRAPHICS_SDL)

som :		SOM.cpp
	g++ -o $(BIN_DIR)som SOM.cpp $(LIB_OS) $(LIB_STD) $(LIB_GRAPHICS_SDL)

sdlgl :		SDL_GL_Demo.cpp
	g++ -o $(BIN_DIR)sdlgl SDL_GL_Demo.cpp $(LIB_OS) $(LIB_STD) $(LIB_GRAPHICS_SDL) $(LIB_GRAPHICS_OPENGL)


test3d :	InteractiveDemo.cpp
	g++ -o $(BIN_DIR)test3d InteractiveDemo.cpp $(LIB_OS) $(LIB_STD) $(LIB_GRAPHICS_OPENGL)

testGrids :	$(TEST_DIR)testrunner.cpp
	g++ -o $(BIN_DIR)testgrids $(TEST_DIR)testrunner.cpp $(TEST_DIR)single_grid_test.cpp $(TEST_DIR)hierarchal_grid_test.cpp $(TEST_DIR)shared_ptr_test.cpp -std=c++0x

ghsomtest:	$(TEST_DIR)ghsomTest.cpp
	g++ -o $(BIN_DIR)ghsomtest $(TEST_DIR)ghsomTest.cpp -std=c++0x -lboost_unit_test_framework

all :	$(BIN_DIR)typhoon
	g++ -o $(BIN_DIR)typhoon $(TEST_DIR)testrunner.cpp -std=c++0x
