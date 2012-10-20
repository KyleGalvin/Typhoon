SHELL = /bin/sh

RELATIVE_ROOT_DIR = ./
TEST_DIR = ./unittests/
BIN_DIR = ./bin/

.SUFFIXES:
.SUFFIXES: .cpp .o

kinect:		KinectDemo.cpp
	g++ -o $(BIN_DIR)kinect KinectDemo.cpp -std=c++0x -lglut -lGL -lGLU -lSDL -lSDL_image

somgl :		somGL.cpp
	g++ -o $(BIN_DIR)somgl somGL.cpp -std=c++0x -lglut -lGL -lGLU -lSDL -lSDL_image

som :		SOM.cpp
	g++ -o $(BIN_DIR)som SOM.cpp -lSDL -lSDL_image -lSDL_gfx -std=c++0x

sdlgl :		SDL_GL_Demo.cpp
	g++ -o $(BIN_DIR)sdlgl SDL_GL_Demo.cpp -std=c++0x -lglut -lGL -lGLU -lSDL -lSDL_image

test3d :	InteractiveDemo.cpp
	g++ -o $(BIN_DIR)test3d InteractiveDemo.cpp -std=c++0x -lglut -lGL -lGLU -lSDL

testGrids :	$(TEST_DIR)testrunner.cpp
	g++ -o $(BIN_DIR)testgrids $(TEST_DIR)testrunner.cpp $(TEST_DIR)single_grid_test.cpp $(TEST_DIR)hierarchal_grid_test.cpp $(TEST_DIR)shared_ptr_test.cpp -std=c++0x

$(TEST_DIR)testrunner.cpp : $(TEST_DIR)single_grid_test.cpp $(TEST_DIR)hierarchal_grid_test.cpp $(TEST_DIR)shared_ptr_test.cpp
	cd $(TEST_DIR) && \
	cxxtestgen --root --error-printer -o testrunner.cpp && \
	cd $(RELATIVE_ROOT_DIR)

$(TEST_DIR)shared_ptr_test.cpp : $(TEST_DIR)shared_ptr_test.ctt
	cd $(TEST_DIR) && \
	cxxtestgen --part --error-printer -o shared_ptr_test.cpp shared_ptr_test.ctt && \ 
	cd $(RELATIVE_ROOT_DIR)

$(TEST_DIR)single_grid_test.cpp : $(TEST_DIR)single_grid_test.ctt
	cd $(TEST_DIR) && \
	cxxtestgen --part --error-printer -o single_grid_test.cpp single_grid_test.ctt && \
	cd $(RELATIVE_ROOT_DIR)

$(TEST_DIR)hierarchal_grid_test.cpp : $(TEST_DIR)hierarchal_grid_test.ctt $(TEST_DIR)hierarchal_grid_debug_extension.cpp
	cd $(TEST_DIR) && \
	cxxtestgen --part --error-printer -o hierarchal_grid_test.cpp hierarchal_grid_test.ctt && \
	cd $(RELATIVE_ROOT_DIR)

all :	$(BIN_DIR)typhoon
	g++ -o $(BIN_DIR)typhoon $(TEST_DIR)testrunner.cpp -std=c++0x
