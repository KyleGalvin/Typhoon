UNAME := $(shell uname)

SHELL = /bin/sh

RELATIVE_ROOT_DIR = ./
TEST_DIR = ./unittests/
BIN_DIR = ./bin/

.SUFFIXES:
.SUFFIXES: .cpp .o
ifeq ($(UNAME), Linux)
game:		game.cpp
	g++ -o $(BIN_DIR)game game.cpp -lSDL -lSDL_image -std=c++0x

endif

ifeq ($(UNAME), CYGWIN_NT-6.1-WOW64)
game:		game.cpp
	g++ -o $(BIN_DIR)game game.cpp -lmingw32 -lSDLMain -lSDL -lSDL_image -mwindows -std=c++0x -I C:/cygwin/usr/include/ -L C:/cygwin/usr/local/lib

endif


kinect:		KinectDemo.cpp
	g++ -o $(BIN_DIR)kinect KinectDemo.cpp -std=c++0x -lglut -lGL -lGLU -lSDL -lSDL_image -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_ts -lopencv_video -I/usr/include/ni -L/home/kgee/kinect/OpenNI-Bin-Dev-Linux-x64-v1.5.4.0/Lib -lOpenNI
sdl :		SDLDemo.cpp
	g++ -o $(BIN_DIR)sdl SDLDemo.cpp -lSDL -lSDL_image -std=c++0x
	
ghsom :		ghsom.cpp
	g++ -o $(BIN_DIR)ghsom ghsom.cpp -lSDL -lSDL_image -lSDL_gfx -std=c++0x

som :		SOM.cpp
	g++ -o $(BIN_DIR)som SOM.cpp -lSDL -lSDL_image -lSDL_gfx -std=c++0x

winheaders :	headers.cpp
	g++ -o $(BIN_DIR)winheaders headers.cpp -lmingw32 -lSDLmain -lSDL -lSDL_image -std=c++0x -lglut -lGL -lGLU -mwindows -I C:/cygwin/usr/include/ -L C:/Windows/System32/

sdlgl :		SDL_GL_Demo.cpp
	g++ -o $(BIN_DIR)sdlgl SDL_GL_Demo.cpp -std=c++0x -lglut -lGL -lGLU -lSDL -lSDL_image

test3d :	InteractiveDemo.cpp
	g++ -o $(BIN_DIR)test3d InteractiveDemo.cpp -std=c++0x -lglut -lGL -lGLU -lSDL

testGrids :	$(TEST_DIR)testrunner.cpp
	g++ -o $(BIN_DIR)testgrids $(TEST_DIR)testrunner.cpp $(TEST_DIR)single_grid_test.cpp $(TEST_DIR)hierarchal_grid_test.cpp $(TEST_DIR)shared_ptr_test.cpp -std=c++0x

ghsomtest:	$(TEST_DIR)ghsomTest.cpp
	g++ -o $(BIN_DIR)ghsomtest $(TEST_DIR)ghsomTest.cpp -std=c++0x -lboost_unit_test_framework

all :	$(BIN_DIR)typhoon
	g++ -o $(BIN_DIR)typhoon $(TEST_DIR)testrunner.cpp -std=c++0x
