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
#include "shared_ptr_test.cpp"

static TestSuite suite_TestSuite;

static CxxTest::List Tests_TestSuite = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestSuite( "shared_ptr_test.cpp", 5, "TestSuite", suite_TestSuite, Tests_TestSuite );

static class TestDescription_suite_TestSuite_testmyTest : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSuite_testmyTest() : CxxTest::RealTestDescription( Tests_TestSuite, suiteDescription_TestSuite, 6, "testmyTest" ) {}
 void runTest() { suite_TestSuite.testmyTest(); }
} testDescription_suite_TestSuite_testmyTest;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
