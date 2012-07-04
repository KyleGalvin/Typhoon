SHELL = /bin/sh

RELATIVE_ROOT_DIR = ./
TEST_DIR = ./unittests/
BIN_DIR = ./bin/

.SUFFIXES:
.SUFFIXES: .cpp .o

test :	$(TEST_DIR)testrunner.cpp
	g++ -o $(BIN_DIR)test $(TEST_DIR)testrunner.cpp $(TEST_DIR)single_grid_test.cpp $(TEST_DIR)hierarchal_grid_test.cpp $(TEST_DIR)shared_ptr_test.cpp -std=c++0x

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

$(TEST_DIR)hierarchal_grid_test.cpp : $(TEST_DIR)hierarchal_grid_test.ctt
	cd $(TEST_DIR) && \
	cxxtestgen --part --error-printer -o hierarchal_grid_test.cpp hierarchal_grid_test.ctt && \
	cd $(RELATIVE_ROOT_DIR)

all :	$(BIN_DIR)typhoon
	g++ -o $(BIN_DIR)typhoon $(TEST_DIR)testrunner.cpp -std=c++0x
