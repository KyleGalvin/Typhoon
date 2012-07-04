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

bool suite_TestSuiteSharedPtr_init = false;
#include "shared_ptr_test.ctt"

static TestSuiteSharedPtr suite_TestSuiteSharedPtr;

static CxxTest::List Tests_TestSuiteSharedPtr = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestSuiteSharedPtr( "shared_ptr_test.ctt", 7, "TestSuiteSharedPtr", suite_TestSuiteSharedPtr, Tests_TestSuiteSharedPtr );

static class TestDescription_suite_TestSuiteSharedPtr_testSharedPtr : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSuiteSharedPtr_testSharedPtr() : CxxTest::RealTestDescription( Tests_TestSuiteSharedPtr, suiteDescription_TestSuiteSharedPtr, 9, "testSharedPtr" ) {}
 void runTest() { suite_TestSuiteSharedPtr.testSharedPtr(); }
} testDescription_suite_TestSuiteSharedPtr_testSharedPtr;

