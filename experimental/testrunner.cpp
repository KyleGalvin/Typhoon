/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    status = CxxTest::Main<CxxTest::ErrorPrinter>( tmp, argc, argv );
    return status;
}
bool suite_TestSuite_init = false;
#include "shared_ptr_test.ctt"

static TestSuite suite_TestSuite;

static CxxTest::List Tests_TestSuite = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestSuite( "shared_ptr_test.ctt", 7, "TestSuite", suite_TestSuite, Tests_TestSuite );

static class TestDescription_suite_TestSuite_testSharedPtr : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSuite_testSharedPtr() : CxxTest::RealTestDescription( Tests_TestSuite, suiteDescription_TestSuite, 8, "testSharedPtr" ) {}
 void runTest() { suite_TestSuite.testSharedPtr(); }
} testDescription_suite_TestSuite_testSharedPtr;

static class TestDescription_suite_TestSuite_testInitializeBoundingBox : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSuite_testInitializeBoundingBox() : CxxTest::RealTestDescription( Tests_TestSuite, suiteDescription_TestSuite, 25, "testInitializeBoundingBox" ) {}
 void runTest() { suite_TestSuite.testInitializeBoundingBox(); }
} testDescription_suite_TestSuite_testInitializeBoundingBox;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
