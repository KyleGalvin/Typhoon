SHELL = /bin/sh

RELATIVE_ROOT_DIR = ./
TEST_DIR = ./experimental/
BIN_DIR = ./bin/

.SUFFIXES:
.SUFFIXES: .cpp .o

test :	$(TEST_DIR)testrunner.cpp
	g++ -o $(BIN_DIR)test $(TEST_DIR)testrunner.cpp -std=c++0x

$(TEST_DIR)testrunner.cpp : $(TEST_DIR)shared_ptr_test.ctt
	cd $(TEST_DIR) && cxxtestgen --error-printer -o testrunner.cpp shared_ptr_test.ctt && cd $(RELATIVE_ROOT_DIR)

all :	$(BIN_DIR)typhoon
	g++ -o $(BIN_DIR)typhoon $(TEST_DIR)testrunner.cpp -std=c++0x
